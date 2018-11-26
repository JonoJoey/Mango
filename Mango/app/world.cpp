#include "world.h"

#include <fstream>
#include <filesystem>


bool World::DoesWorldExist(std::string world_path)
{
	return std::filesystem::exists(world_path) && std::filesystem::exists(world_path + "/metadata.data") && std::filesystem::exists(world_path + "/chunks");
}
void World::CreateNewWorld(std::string world_path, uint32_t seed)
{
	// empty or create new directory
	if (std::filesystem::exists(world_path))
		std::filesystem::remove_all(world_path);

	std::filesystem::create_directory(world_path);
	std::filesystem::create_directory(world_path + "/chunks");

	// metadata
	std::fstream file(world_path + "/metadata.data", std::ios::out | std::ios::trunc);
	ASSERT(file);

	file << "[seed][" << std::to_string(seed) << "]" << std::endl;
	file << "[time][" << std::to_string(0.f) << "]" << std::endl;

	file.close();
}

bool World::Setup(std::string world_path, std::unordered_map<std::string, BLOCK_ID> block_map)
{
	if (!std::filesystem::exists(world_path))
		return false;

	// null is the only block actually required
	if (block_map.find("null") == block_map.end())
		return false;

	m_ray_tracer.SetWorld(this);
	m_block_map = block_map;
	m_world_path = world_path;

	std::deque<std::string> file_data;

	// metadata
	{
		std::fstream file(world_path + "/metadata.data", std::ios::in);
		if (!file)
			return false;

		std::string line;
		while (std::getline(file, line))
		{
			if (!line.empty())
				file_data.push_back(line);
		}

		file.close();
	}

	const auto ParseString = [](const std::string& string) -> std::deque<std::string>
	{
		std::deque<std::string> args;

		bool start = false;
		std::string cache;
		for (const char c : string)
		{
			if (c == '[')
				start = true;
			else if (start && c == ']')
			{
				start = false;
				args.push_back(cache);
				cache.clear();
			}
			else if (start)
				cache += c;
		}

		return args;
	};
	const auto FindData = [ParseString](const std::deque<std::string>& file_data, const std::string& data) -> std::deque<std::string>
	{
		for (auto string : file_data)
		{
			auto args = ParseString(string);
			if (args.empty())
				continue;

			if (args.front() == data)
			{
				args.pop_front();
				return args;
			}
		}

		return std::deque<std::string>();
	};

	if (const auto seed = FindData(file_data, "seed"); !seed.empty())
		m_perlin_noise.reseed(stoul(seed.front()));

	return true;
}
void World::Release()
{
	for (auto chunk : m_chunks)
	{
		const auto& edited_blocks = m_edited_blocks.find(chunk.first);
		if (edited_blocks != m_edited_blocks.end())
		{
			SaveChunk(chunk.second->GetX(), chunk.second->GetZ(), edited_blocks->second, m_world_path);
			m_edited_blocks.erase(chunk.first);
		}
	}

	m_edited_blocks.clear();
	m_update_chunks.clear();
	m_load_chunks.clear();
	m_chunks.clear();
	m_render_chunks.clear();
}

void World::Render()
{

}
void World::Update(glm::fvec3 position)
{
	const int x_chunk = Chunk::PositionXToChunk(int(position.x)),
		z_chunk = Chunk::PositionZToChunk(int(position.z));

	const auto DoesChunkExist = [this](int x, int z) -> bool
	{
		return m_chunks.find(PackChunk(x, z)) != m_chunks.end();
	};

	// when entering a new chunk
	if (static uint64_t last_chunk = 0xFFFFFFFFFFFFFFFF; last_chunk != PackChunk(x_chunk, z_chunk))
	{
		last_chunk = PackChunk(x_chunk, z_chunk);

		// delete chunks that are too far
		for (auto it = m_chunks.begin(); it != m_chunks.end();)
		{
			int x, z;
			UnpackChunk(it->first, x, z);
			if (abs(x - x_chunk) > RENDER_DISTANCE + 1 || abs(z - z_chunk) > RENDER_DISTANCE + 1)
			{
				SaveChunk(x, z, m_edited_blocks[it->first], m_world_path);

				it = m_chunks.erase(it);
				continue;
			}
			++it;
		}

		// clear
		m_render_chunks.clear();
		m_update_chunks.clear();
		m_load_chunks.clear();

		// add chunks to load list
		std::unordered_map<uint64_t, bool> test;
		for (int d = 0; d <= RENDER_DISTANCE + 1; d++)
		{
			for (int x = -d; x <= d; x++)
			{
				for (int z = -d; z <= d; z++)
				{
					const auto packed_chunk = PackChunk(x, z);
					if (test.find(packed_chunk) != test.end())
						continue;

					// chunk already exists
					if (auto it = m_chunks.find(PackChunk(x_chunk + x, z_chunk + z)); it != m_chunks.end())
					{
						if (abs(x) <= RENDER_DISTANCE && abs(z) <= RENDER_DISTANCE)
						{
							m_render_chunks.push_back(it->second);

							if (it->second->DoesNeedUpdate())
								m_update_chunks.push_back(&*it->second);
						}

						test[packed_chunk] = true;
						continue;
					}

					test[packed_chunk] = true;
					m_load_chunks.push_back(packed_chunk);
				}
			}
		}
	}

	// load chunks
	if (!m_load_chunks.empty())
	{
		int x, z;
		UnpackChunk(m_load_chunks.front(), x, z);

		auto chunk = NewChunk(x_chunk + x, z_chunk + z);
		LoadChunk(x + x_chunk, z + z_chunk, &*chunk);
		if (x >= -RENDER_DISTANCE && x <= RENDER_DISTANCE && z >= -RENDER_DISTANCE && z <= RENDER_DISTANCE)
		{
			m_render_chunks.push_back(chunk);
			m_update_chunks.push_back(&*chunk);
		}

		m_load_chunks.pop_front();
	}

	// update chunks
	for (size_t i = 0; i < m_update_chunks.size(); i++)
	{
		auto chunk = m_update_chunks[i];
		if (!DoesChunkExist(chunk->GetX() + 1, chunk->GetZ()) || !DoesChunkExist(chunk->GetX() - 1, chunk->GetZ()) ||
			!DoesChunkExist(chunk->GetX(), chunk->GetZ() + 1) || !DoesChunkExist(chunk->GetX(), chunk->GetZ() - 1))
			continue;

		chunk->Update(m_chunks);
		m_update_chunks.erase(m_update_chunks.begin() + i);
		break;
	}

	// update entities
	UpdateEntities();
}
void World::UpdateEntities()
{

}

void World::GenerateChunk(Chunk* chunk)
{
	static constexpr int VARIANCE = 30;
	static constexpr int MAX_HEIGHT = (Chunk::HEIGHT / 2) + (VARIANCE / 2);
	static constexpr int MIN_HEIGHT = (Chunk::HEIGHT / 2) - (VARIANCE / 2);
	static constexpr float MULTIPLIER = 30.f;

	BLOCK_ID cobblestone = m_block_map["null"];
	if (auto it = m_block_map.find("cobblestone"); it != m_block_map.end())
		cobblestone = it->second;

	BLOCK_ID grass = m_block_map["null"];
	if (auto it = m_block_map.find("grass"); it != m_block_map.end())
		grass = it->second;

	for (int x = 0; x < Chunk::WIDTH; x++)
	{
		for (int z = 0; z < Chunk::DEPTH; z++)
		{
			const auto noise = m_perlin_noise.noise(float(x + (chunk->GetX() * Chunk::WIDTH)) / MULTIPLIER, float(z + (chunk->GetZ() * Chunk::DEPTH)) / MULTIPLIER);
			const int height = MIN_HEIGHT + int((noise + 1.f) * 0.5f * (MAX_HEIGHT - MIN_HEIGHT));

			for (int i = 0; i < height; i++)
			{
				if (height - i <= 4)
					chunk->SetBlock(x, i, z, Block::Create(grass));
				else
					chunk->SetBlock(x, i, z, Block::Create(cobblestone));
			}
		}
	}
}
void World::LoadChunk(int x, int z, Chunk* chunk)
{
	GenerateChunk(chunk);

	const auto ParseString = [](const std::string& string) -> std::deque<std::string>
	{
		std::deque<std::string> args;

		bool start = false;
		std::string cache;
		for (const char c : string)
		{
			if (c == '[')
				start = true;
			else if (start && c == ']')
			{
				start = false;
				args.push_back(cache);
				cache.clear();
			}
			else if (start)
				cache += c;
		}

		return args;
	};

	std::fstream file(m_world_path + "/chunks/chunk_[" + std::to_string(x) + "][" + std::to_string(z) + "]", std::ios::in);
	if (!file)
		return;

	const uint64_t packed_chunk = PackChunk(x, z);

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty())
			continue;

		const auto args = ParseString(line);
		if (args.empty())
			continue;

		ASSERT(args.size() == 3);
		uint32_t position = stoul(args[0]);
		int is_active = stoi(args[1]);
		int block_id = stoi(args[2]);

		EditedBlock edited_block;
		edited_block.m_position = position;
		edited_block.m_block.m_is_active = is_active;
		edited_block.m_block.m_block_id = block_id;
		m_edited_blocks[packed_chunk].push_back(edited_block);

		int block_x, block_y, block_z;
		EditedBlock::UnpackPosition(position, block_x, block_y, block_z);
		chunk->SetBlock(block_x, block_y, block_z, edited_block.m_block);
	}

	file.close();
}

void World::SaveChunk(int x, int z, std::deque<EditedBlock> edited_blocks, std::string world_path)
{
	if (edited_blocks.empty())
		return;

	std::fstream file(world_path + "/chunks/chunk_[" + std::to_string(x) + "][" + std::to_string(z) + "]", std::ios::out | std::ios::trunc);
	ASSERT(file);

	for (auto& block : edited_blocks)
		file << "[" << block.m_position << "][" << int(block.m_block.m_is_active) << "][" << int(block.m_block.m_block_id) << "]" << std::endl;

	file.close();
}

void World::EditBlock(int x, int y, int z, const Block& block)
{
	if (y < 0 || y >= Chunk::HEIGHT - 1)
		return;

	const int chunk_x = Chunk::PositionXToChunk(x),
		chunk_z = Chunk::PositionZToChunk(z);

	auto chunk = GetChunk(chunk_x, chunk_z);
	if (!chunk)
	{
		chunk = &*NewChunk(chunk_x, chunk_z);
		LoadChunk(chunk_x, chunk_z, chunk);
	}

	int new_x = x % Chunk::WIDTH;
	int new_z = z % Chunk::DEPTH;

	if (new_x < 0)
		new_x += Chunk::WIDTH;
	if (new_z < 0)
		new_z += Chunk::DEPTH;

	if (chunk->GetBlock(new_x, y, new_z) == block)
		return;

	EditedBlock edited_block;
	edited_block.m_block = block;
	edited_block.m_position = EditedBlock::PackPosition(new_x, y, new_z);

	m_update_chunks.push_front(chunk);
	m_edited_blocks[PackChunk(chunk_x, chunk_z)].push_back(edited_block);

	chunk->SetBlock(new_x, y, new_z, block);
}
bool World::GetBlock(int x, int y, int z, Block& block)
{
	if (y < 0 || y >= Chunk::HEIGHT - 1)
		return false;

	const int chunk_x = Chunk::PositionXToChunk(x),
		chunk_z = Chunk::PositionZToChunk(z);

	auto chunk = GetChunk(chunk_x, chunk_z);
	if (!chunk)
		return false;

	int new_x = x % Chunk::WIDTH;
	int new_z = z % Chunk::DEPTH;

	if (new_x < 0)
		new_x += Chunk::WIDTH;
	if (new_z < 0)
		new_z += Chunk::DEPTH;

	block = chunk->GetBlock(new_x, y, new_z);
	return true;
}

Chunk* World::GetChunk(int x, int z)
{
	if (auto chunk = m_chunks.find(PackChunk(x, z)); chunk != m_chunks.end())
		return &*chunk->second;

	return nullptr;
}