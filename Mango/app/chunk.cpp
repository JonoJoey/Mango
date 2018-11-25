#include "chunk.h"

#include <fstream>
#include <filesystem>
#include <thread>


void Chunk::Setup(int x, int z)
{
	m_x = x;
	m_z = z;

	m_blocks = new Block[WIDTH * HEIGHT * DEPTH];
	memset(m_blocks, 0, sizeof(Block) * WIDTH * HEIGHT * DEPTH);

	m_model.Setup(GL_TRIANGLES, 0, GL_UNSIGNED_INT, nullptr);
	m_model.AddVBO().Setup(0, nullptr);
	m_model.AddVBO().Setup(0, nullptr);
}
void Chunk::Release()
{
	if (!m_blocks)
		return;

	m_model.Release();

	delete[] m_blocks;
	m_blocks = nullptr;
}
bool Chunk::Update(std::unordered_map<uint64_t, std::shared_ptr<Chunk>> chunks)
{
	if (!m_needs_update)
		return false;
	m_needs_update = false;

	std::vector<float> vertices;
	std::vector<float> tex_coords;
	std::vector<unsigned int> indices;

	const auto MakeBlock = [&vertices, &tex_coords, &indices, &chunks, this](int x, int y, int z) -> void
	{
		if (!GetBlock(x, y, z).m_is_active)
			return;

		const auto FindChunk = [&chunks](int chunk_x, int chunk_z) -> Chunk*
		{
			if (auto it = chunks.find(World::PackChunk(chunk_x, chunk_z)); it != chunks.end())
				return &*it->second;

			return nullptr;
		};
		const auto IsBlockActive = [FindChunk, this](int x, int y, int z, bool bx, bool bz) -> bool
		{
			const int chunk_x = m_x,
				chunk_z = m_z;

			// false = draw
			// true = dont draw

			if (bx)
			{
				if (x < 0)
				{
					auto chunk = FindChunk(chunk_x - 1, chunk_z);
					if (!chunk)
						return false;

					return chunk->GetBlock(WIDTH - 1, y, z).m_is_active;

				}
				else if (x >= WIDTH)
				{
					//return false;

					auto chunk = FindChunk(chunk_x + 1, chunk_z);
					if (!chunk)
						return false;

					return chunk->GetBlock(0, y, z).m_is_active;
				}
				else
					return GetBlock(x, y, z).m_is_active;
			}
			else if (bz)
			{
				if (z < 0)
				{
					auto chunk = FindChunk(chunk_x, chunk_z - 1);
					if (!chunk)
						return false;

					return chunk->GetBlock(x, y, DEPTH - 1).m_is_active;

				}
				else if (z >= WIDTH)
				{
					//return false;

					auto chunk = FindChunk(chunk_x, chunk_z + 1);
					if (!chunk)
						return false;

					return chunk->GetBlock(x, y, 0).m_is_active;
				}
				else
					return GetBlock(x, y, z).m_is_active;
			}

			return true;
		};

		const float f_x = float(x),
			f_y = float(y),
			f_z = float(z);

		// front face
		if (!IsBlockActive(x, y, z + 1, false, true))
		{
			const unsigned int start = vertices.size() / 3;

			vertices.push_back(f_x);
			vertices.push_back(f_y);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(0.f);
			tex_coords.push_back(0.f);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(1.f);
			tex_coords.push_back(0.f);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(1.f);
			tex_coords.push_back(1.f);

			vertices.push_back(f_x);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(0.f);
			tex_coords.push_back(1.f);

			indices.push_back(start + 0);
			indices.push_back(start + 1);
			indices.push_back(start + 2);

			indices.push_back(start + 0);
			indices.push_back(start + 2);
			indices.push_back(start + 3);
		}

		// back
		if (!IsBlockActive(x, y, z - 1, false, true))
		{
			const unsigned int start = vertices.size() / 3;

			vertices.push_back(f_x);
			vertices.push_back(f_y);
			vertices.push_back(f_z);
			tex_coords.push_back(1.f);
			tex_coords.push_back(0.f);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y);
			vertices.push_back(f_z);
			tex_coords.push_back(0.f);
			tex_coords.push_back(0.f);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z);
			tex_coords.push_back(0.f);
			tex_coords.push_back(1.f);

			vertices.push_back(f_x);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z);
			tex_coords.push_back(1.f);
			tex_coords.push_back(1.f);

			indices.push_back(start + 1);
			indices.push_back(start + 0);
			indices.push_back(start + 3);

			indices.push_back(start + 1);
			indices.push_back(start + 3);
			indices.push_back(start + 2);
		}

		// right
		if (!IsBlockActive(x + 1, y, z, true, false))
		{
			const unsigned int start = vertices.size() / 3;

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(0.f);
			tex_coords.push_back(0.f);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y);
			vertices.push_back(f_z);
			tex_coords.push_back(1.f);
			tex_coords.push_back(0.f);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z);
			tex_coords.push_back(1.f);
			tex_coords.push_back(1.f);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(0.f);
			tex_coords.push_back(1.f);

			indices.push_back(start + 0);
			indices.push_back(start + 1);
			indices.push_back(start + 2);

			indices.push_back(start + 0);
			indices.push_back(start + 2);
			indices.push_back(start + 3);
		}

		// left
		if (!IsBlockActive(x - 1, y, z, true, false))
		{
			const unsigned int start = vertices.size() / 3;

			vertices.push_back(f_x);
			vertices.push_back(f_y);
			vertices.push_back(f_z);
			tex_coords.push_back(0.f);
			tex_coords.push_back(0.f);

			vertices.push_back(f_x);
			vertices.push_back(f_y);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(1.f);
			tex_coords.push_back(0.f);

			vertices.push_back(f_x);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(1.f);
			tex_coords.push_back(1.f);

			vertices.push_back(f_x);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z);
			tex_coords.push_back(0.f);
			tex_coords.push_back(1.f);

			indices.push_back(start + 0);
			indices.push_back(start + 1);
			indices.push_back(start + 2);

			indices.push_back(start + 0);
			indices.push_back(start + 2);
			indices.push_back(start + 3);
		}

		// top
		if (y >= HEIGHT - 1 || !GetBlock(x, y + 1, z).m_is_active)
		{
			const unsigned int start = vertices.size() / 3;

			vertices.push_back(f_x);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(0.f);
			tex_coords.push_back(0.f);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(1.f);
			tex_coords.push_back(0.f);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z);
			tex_coords.push_back(1.f);
			tex_coords.push_back(1.f);

			vertices.push_back(f_x);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z);
			tex_coords.push_back(0.f);
			tex_coords.push_back(1.f);

			indices.push_back(start + 0);
			indices.push_back(start + 1);
			indices.push_back(start + 2);

			indices.push_back(start + 0);
			indices.push_back(start + 2);
			indices.push_back(start + 3);
		}

		// bottom
		if (y <= 0 || !GetBlock(x, y - 1, z).m_is_active)
		{
			const unsigned int start = vertices.size() / 3;

			vertices.push_back(f_x);
			vertices.push_back(f_y);
			vertices.push_back(f_z);
			tex_coords.push_back(0.f);
			tex_coords.push_back(0.f);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y);
			vertices.push_back(f_z);
			tex_coords.push_back(1.f);
			tex_coords.push_back(0.f);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(1.f);
			tex_coords.push_back(1.f);

			vertices.push_back(f_x);
			vertices.push_back(f_y);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(0.f);
			tex_coords.push_back(1.f);

			indices.push_back(start + 0);
			indices.push_back(start + 1);
			indices.push_back(start + 2);

			indices.push_back(start + 0);
			indices.push_back(start + 2);
			indices.push_back(start + 3);
		}
	};

	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int z = 0; z < DEPTH; z++)
				MakeBlock(x, y, z);
		}
	}

	// empty
	if (indices.empty())
		return false;

	auto positions_buffer = &m_model.GetVBOs()[0];
	auto tex_coords_buffer = &m_model.GetVBOs()[1];
	auto index_buffer = &m_model.GetIBO();

	m_model.GetVAO().Bind();

	// index buffer
	index_buffer->Bind();
	index_buffer->SetCount(indices.size());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

	// positions
	positions_buffer->Bind();
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
	Mango::VertexArray::EnableAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	// tex_coords
	tex_coords_buffer->Bind();
	glBufferData(GL_ARRAY_BUFFER, tex_coords.size() * sizeof(float), tex_coords.data(), GL_DYNAMIC_DRAW);
	Mango::VertexArray::EnableAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	Mango::VertexArray::Unbind();

	return true;
}

int Chunk::PositionXToChunk(int x)
{
	return (x >= 0) ? (x / WIDTH) : ((x + 1) / WIDTH - 1);
}
int Chunk::PositionZToChunk(int z)
{
	return (z >= 0) ? (z / DEPTH) : ((z + 1) / DEPTH - 1);
}

uint32_t Chunk::PackBlock(int x, int y, int z)
{
	uint32_t block;

	*reinterpret_cast<uint8_t*>(uintptr_t(&block)) = uint8_t(x);
	*reinterpret_cast<uint8_t*>(uintptr_t(&block) + 1) = uint8_t(z);
	*reinterpret_cast<uint16_t*>(uintptr_t(&block) + 2) = uint16_t(y);

	return block;
}
void Chunk::UnpackBlock(uint32_t block, int& x, int& y, int& z)
{
	x = int(*reinterpret_cast<uint8_t*>(uintptr_t(&block)));
	z = int(*reinterpret_cast<uint8_t*>(uintptr_t(&block) + 1));
	y = int(*reinterpret_cast<uint16_t*>(uintptr_t(&block) + 2));
}

void Chunk::SetBlock(int x, int y, int z, const Block& block)
{ 
	m_needs_update = true;
	m_blocks[x + (y * WIDTH) + (z * WIDTH * HEIGHT)] = block; 
}
Block Chunk::GetBlock(int x, int y, int z) 
{ 
	return m_blocks[x + (y * WIDTH) + (z * WIDTH * HEIGHT)];
}


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

void World::Setup(std::string world_path)
{
	ASSERT(std::filesystem::exists(world_path));

	m_world_path = world_path;

	std::deque<std::string> file_data;

	// metadata
	{
		std::fstream file(world_path + "/metadata.data", std::ios::in);
		ASSERT(file);

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

		std::deque<std::thread> threads;

		// delete chunks that are too far
		for (auto it = m_chunks.begin(); it != m_chunks.end();)
		{
			int x, z;
			UnpackChunk(it->first, x, z);
			if (abs(x - x_chunk) > RENDER_DISTANCE + 1 || abs(z - z_chunk) > RENDER_DISTANCE + 1)
			{
				// spawn a new thread to save the chunk
				threads.emplace_back(SaveChunk, x, z, m_edited_blocks[it->first], m_world_path);

				it = m_chunks.erase(it);
				continue;
			}
			++it;
		}

		// wait for threads to finish
		for (auto& t : threads)
			t.join();

		// load a new chunk
		const auto LoadNewChunk = [x_chunk, z_chunk, this](int x, int z, std::shared_ptr<Chunk> chunk) -> void
		{
			LoadChunk(x + x_chunk, z + z_chunk, &*chunk);
			if (x >= -RENDER_DISTANCE && x <= RENDER_DISTANCE && z >= -RENDER_DISTANCE && z <= RENDER_DISTANCE)
			{
				m_render_chunks.push_back(chunk);
				m_update_chunks.push_back(&*chunk);
			}
		};

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

	static bool bool_switch = false;
	bool_switch = !bool_switch;

	// load a chunk every tick
	if (bool_switch)
	{
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
	}
	else
	{
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
	}
}
void World::Release()
{
	for (auto chunk : m_chunks)
	{
		const auto& edited_blocks = m_edited_blocks.find(chunk.first);
		if (edited_blocks != m_edited_blocks.end())
		{
			auto edited_blocks_copy = edited_blocks->second;
			m_edited_blocks.erase(chunk.first);

			SaveChunk(chunk.second->GetX(), chunk.second->GetZ(), edited_blocks_copy, m_world_path);
		}
	}

	m_edited_blocks.clear();
	m_chunks.clear();
	m_render_chunks.clear();
}

void World::GenerateChunk(Chunk* chunk)
{
	static constexpr int MIN_HEIGHT = 256 / 3;
	static constexpr int MAX_HEIGHT = 256 - (256 / 3);
	static constexpr float MULTIPLIER = 30.f;

	for (int x = 0; x < Chunk::WIDTH; x++)
	{
		for (int z = 0; z < Chunk::DEPTH; z++)
		{
			const auto noise = m_perlin_noise.noise(float(x + (chunk->GetX() * Chunk::WIDTH)) / MULTIPLIER, float(z + (chunk->GetZ() * Chunk::DEPTH)) / MULTIPLIER);
			const int height = MIN_HEIGHT + int((noise + 1.f) * 0.5f * (MAX_HEIGHT - MIN_HEIGHT));
	
			for (int i = 0; i < height; i++)
				chunk->SetBlock(x, i, z, Block::Create(1));
		}
	}
}
void World::LoadChunk(int x, int z, Chunk* chunk)
{
	GenerateChunk(chunk);
	//m_update_chunks.push_back(chunk);

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

	//auto c = GetChunk(x + 1, z);
	//if (c)
	//{
	//	c->SetNeedsUpdate(true);
	//	m_update_chunks.emplace_back(c);
	//}
	//
	//c = GetChunk(x - 1, z);
	//if (c)
	//{
	//	c->SetNeedsUpdate(true);
	//	m_update_chunks.emplace_back(c);
	//}
	//
	//c = GetChunk(x, z + 1);
	//if (c)
	//{
	//	c->SetNeedsUpdate(true);
	//	m_update_chunks.emplace_back(c);
	//}
	//
	//c = GetChunk(x, z - 1);
	//if (c)
	//{
	//	c->SetNeedsUpdate(true);
	//	m_update_chunks.emplace_back(c);
	//}
}

uint64_t World::PackChunk(int x, int z)
{
	uint64_t packed_chunk;
	*reinterpret_cast<int32_t*>(uintptr_t(&packed_chunk)) = int32_t(x);
	*reinterpret_cast<int32_t*>(uintptr_t(&packed_chunk) + 4) = int32_t(z);
	return packed_chunk;
}
void World::UnpackChunk(uint64_t chunk, int& x, int& z)
{
	x = *reinterpret_cast<int32_t*>(uintptr_t(&chunk));
	z = *reinterpret_cast<int32_t*>(uintptr_t(&chunk) + 4);
}
void World::SaveChunk(int x, int z, std::deque<EditedBlock> edited_blocks, std::string world_path)
{
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
		return;
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

	m_update_chunks.push_back(chunk);
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

	block = chunk->GetBlock(new_x, y, new_z);
	return true;
}

Chunk* World::GetChunk(int x, int z)
{
	if (auto chunk = m_chunks.find(PackChunk(x, z)); chunk != m_chunks.end())
		return &*chunk->second;

	return nullptr;
}