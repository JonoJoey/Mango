#include "chunk.h"


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
bool Chunk::Update(std::deque<std::shared_ptr<Chunk>>& render_chunks)
{
	if (!m_needs_update)
		return false;

	m_needs_update = false;
	DBG_LOG("[~] chunk: [%i, %i]", m_x, m_z);

	std::vector<float> vertices;
	std::vector<float> tex_coords;
	std::vector<unsigned int> indices;

	const auto MakeBlock = [&vertices, &tex_coords, &indices, &render_chunks, this](int x, int y, int z) -> void
	{
		if (!GetBlock(x, y, z).m_is_active)
			return;

		const auto FindChunk = [&render_chunks](int chunk_x, int chunk_z) -> Chunk*
		{
			for (auto chunk : render_chunks)
			{
				if (chunk->GetX() == chunk_x && chunk->GetZ() == chunk_z)
					return &*chunk;
			}

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
			GenerateChunk(x_chunk, z_chunk);

			auto chunk = GetChunk(x_chunk + 1, z_chunk);
			if (chunk) chunk->SetUpdate();

			chunk = GetChunk(x_chunk - 1, z_chunk);
			if (chunk) chunk->SetUpdate();

			chunk = GetChunk(x_chunk, z_chunk + 1);
			if (chunk) chunk->SetUpdate();

			chunk = GetChunk(x_chunk, z_chunk - 1);
			if (chunk) chunk->SetUpdate();
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
		{
			if (chunk->Update(m_render_chunks))
				break;
		}
	}
}
void World::Release()
{
	m_chunks.clear();
	m_render_chunks.clear();
}

std::shared_ptr<Chunk> World::GenerateChunk(int x, int z)
{
	DBG_LOG("[+] chunk: [%i %i]", x, z);
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