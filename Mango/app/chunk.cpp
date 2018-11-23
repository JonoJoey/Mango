#include "chunk.h"


void Chunk::Setup(int x, int z)
{
	m_x = x;
	m_z = z;

	m_blocks = new Block[WIDTH * HEIGHT * DEPTH];
	memset(m_blocks, 0, sizeof(Block) * WIDTH * HEIGHT * DEPTH);

	m_model.Setup(GL_TRIANGLES, 0, GL_UNSIGNED_INT, nullptr);
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
void Chunk::Update()
{
	if (!m_needs_update)
		return;

	m_needs_update = false;
	DBG_LOG("~chunk: [%i, %i]", m_x, m_z);

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	const auto MakeBlock = [&vertices, &indices, this](int x, int y, int z) -> void
	{
		if (!GetBlock(x, y, z).m_is_active)
			return;

		const unsigned int start = vertices.size() / 3;

		// front face
		if (z <= 0 || !GetBlock(x, y, z - 1).m_is_active)
		{
			indices.push_back(start + 0);
			indices.push_back(start + 2);
			indices.push_back(start + 1);
			indices.push_back(start + 0);
			indices.push_back(start + 3);
			indices.push_back(start + 2);
		}

		// back
		if (z >= DEPTH - 1 || !GetBlock(x, y, z + 1).m_is_active)
		{
			indices.push_back(start + 4);
			indices.push_back(start + 5);
			indices.push_back(start + 6);
			indices.push_back(start + 4);
			indices.push_back(start + 6);
			indices.push_back(start + 7);
		}

		// right
		if (x >= WIDTH - 1 || !GetBlock(x + 1, y, z).m_is_active)
		{
			indices.push_back(start + 1);
			indices.push_back(start + 6);
			indices.push_back(start + 5);
			indices.push_back(start + 1);
			indices.push_back(start + 2);
			indices.push_back(start + 6);
		}

		// left
		if (x <= 0 || !GetBlock(x - 1, y, z).m_is_active)
		{
			indices.push_back(start + 0);
			indices.push_back(start + 4);
			indices.push_back(start + 7);
			indices.push_back(start + 0);
			indices.push_back(start + 7);
			indices.push_back(start + 3);
		}

		// top
		if (y >= HEIGHT - 1 || !GetBlock(x, y + 1, z).m_is_active)
		{
			indices.push_back(start + 7);
			indices.push_back(start + 6);
			indices.push_back(start + 2);
			indices.push_back(start + 7);
			indices.push_back(start + 2);
			indices.push_back(start + 3);
		}

		// bottom
		if (y <= 0 || !GetBlock(x, y - 1, z).m_is_active)
		{
			indices.push_back(start + 4);
			indices.push_back(start + 1);
			indices.push_back(start + 5);
			indices.push_back(start + 4);
			indices.push_back(start + 0);
			indices.push_back(start + 1);
		}

		const float f_x = float(x),
			f_y = float(y),
			f_z = float(z);

		// 0
		vertices.push_back(f_x);
		vertices.push_back(f_y);
		vertices.push_back(f_z);

		// 1
		vertices.push_back(f_x + 1.f);
		vertices.push_back(f_y);
		vertices.push_back(f_z);

		// 2
		vertices.push_back(f_x + 1.f);
		vertices.push_back(f_y + 1.f);
		vertices.push_back(f_z);

		// 3
		vertices.push_back(f_x);
		vertices.push_back(f_y + 1.f);
		vertices.push_back(f_z);

		// 4
		vertices.push_back(f_x);
		vertices.push_back(f_y);
		vertices.push_back(f_z + 1.f);

		// 5
		vertices.push_back(f_x + 1.f);
		vertices.push_back(f_y);
		vertices.push_back(f_z + 1.f);

		// 6
		vertices.push_back(f_x + 1.f);
		vertices.push_back(f_y + 1.f);
		vertices.push_back(f_z + 1.f);

		// 7
		vertices.push_back(f_x);
		vertices.push_back(f_y + 1.f);
		vertices.push_back(f_z + 1.f);
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
		return;

	auto vertex_buffer = &m_model.GetVBOs().front();
	auto index_buffer = &m_model.GetIBO();

	m_model.GetVAO().Bind();

	// index buffer
	index_buffer->Bind();
	index_buffer->SetCount(indices.size());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

	// vertex buffer
	vertex_buffer->Bind();
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
	Mango::VertexArray::EnableAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	Mango::VertexArray::Unbind();
}

int Chunk::PositionXToChunk(int x)
{
	return (x >= 0) ? (x / WIDTH) : ((x + 1) / WIDTH - 1);
}
int Chunk::PositionZToChunk(int z)
{
	return (z >= 0) ? (z / DEPTH) : ((z + 1) / DEPTH - 1);
}


void World::Setup(glm::fvec3 position)
{
	m_perlin_noise.reseed(69);
}
void World::Update(glm::fvec3 position)
{
	const int x_chunk = Chunk::PositionXToChunk(int(position.x)),
		z_chunk = Chunk::PositionZToChunk(int(position.z));

	// update chunks
	{
		const auto DoesChunkExist = [this](int x, int z) -> bool
		{
			for (auto chunk : m_chunks)
			{
				if (chunk->GetX() == x && chunk->GetZ() == z)
					return true;
			}

			return false;
		};

		if (!DoesChunkExist(x_chunk, z_chunk))
		{
			auto chunk = GenerateChunk(x_chunk, z_chunk);

		}
	}

	// update render chunks
	{
		m_render_chunks.clear();

		for (auto chunk : m_chunks)
		{
			if (fabs(chunk->GetX() - x_chunk) <= CHUNK_RADIUS && fabs(chunk->GetZ() - z_chunk) <= CHUNK_RADIUS)
				m_render_chunks.push_back(chunk);
		}

		for (auto chunk : m_render_chunks)
			chunk->Update();
	}
}
void World::Release()
{
	m_chunks.clear();
	m_render_chunks.clear();
}

std::shared_ptr<Chunk> World::GenerateChunk(int x, int z)
{
	DBG_LOG("+chunk: [%i %i]", x, z);
	auto chunk = m_chunks.emplace_back(new Chunk(x, z));

	static constexpr int MIN_HEIGHT = 256 / 4;
	static constexpr int MAX_HEIGHT = 256 - (256 / 4);
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

	return chunk;
}

void World::EditBlock(int x, int y, int z, const Block& block)
{
	if (y < 0 || y >= Chunk::HEIGHT - 1)
		return;

	const int chunk_x = Chunk::PositionXToChunk(x),
		chunk_z = Chunk::PositionZToChunk(z);

	auto chunk = GetChunk(chunk_x, chunk_z);
	if (!chunk)
		chunk = &*GenerateChunk(chunk_x, chunk_z);

	int new_x = x % Chunk::WIDTH;
	int new_z = z % Chunk::DEPTH;

	if (new_x < 0)
		new_x += Chunk::WIDTH;

	if (new_z < 0)
		new_z += Chunk::DEPTH;

	DBG_LOG("[%i %i] %i %i - %i %i", chunk_x, chunk_z, x, z, new_x, new_z);

	chunk->SetBlock(new_x, y, new_z, block);
}
Chunk* World::GetChunk(int x, int z)
{
	for (auto chunk : m_chunks)
	{
		if (chunk->GetX() == x && chunk->GetZ() == z)
			return &*chunk;
	}

	return nullptr;
}