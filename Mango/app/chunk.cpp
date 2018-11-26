#include "chunk.h"



uint64_t PackChunk(int x, int z)
{
	uint64_t packed_chunk;
	*reinterpret_cast<int32_t*>(uintptr_t(&packed_chunk)) = int32_t(x);
	*reinterpret_cast<int32_t*>(uintptr_t(&packed_chunk) + 4) = int32_t(z);
	return packed_chunk;
}
void UnpackChunk(uint64_t chunk, int& x, int& z)
{
	x = *reinterpret_cast<int32_t*>(uintptr_t(&chunk));
	z = *reinterpret_cast<int32_t*>(uintptr_t(&chunk) + 4);
}


void Chunk::Setup(int x, int z)
{
	m_x = x;
	m_z = z;

	m_blocks = new Block[WIDTH * HEIGHT * DEPTH];
	memset(m_blocks, 0, sizeof(Block) * WIDTH * HEIGHT * DEPTH);

	m_model.Setup(GL_TRIANGLES, 0, GL_UNSIGNED_INT, nullptr);
	m_model.AddVBO().Setup(0, nullptr);
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
	std::vector<BLOCK_ID> block_ids;
	std::vector<unsigned int> indices;

	const auto MakeBlock = [&vertices, &tex_coords, &block_ids, &indices, &chunks, this](int x, int y, int z, int block_id) -> void
	{
		if (!GetBlock(x, y, z).m_is_active)
			return;

		const auto FindChunk = [&chunks](int chunk_x, int chunk_z) -> Chunk*
		{
			if (auto it = chunks.find(PackChunk(chunk_x, chunk_z)); it != chunks.end())
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
			block_ids.push_back((block_id * 6) + 0);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(1.f);
			tex_coords.push_back(0.f);
			block_ids.push_back((block_id * 6) + 0);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(1.f);
			tex_coords.push_back(1.f);
			block_ids.push_back((block_id * 6) + 0);

			vertices.push_back(f_x);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(0.f);
			tex_coords.push_back(1.f);
			block_ids.push_back((block_id * 6) + 0);

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
			block_ids.push_back((block_id * 6) + 1);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y);
			vertices.push_back(f_z);
			tex_coords.push_back(0.f);
			tex_coords.push_back(0.f);
			block_ids.push_back((block_id * 6) + 1);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z);
			tex_coords.push_back(0.f);
			tex_coords.push_back(1.f);
			block_ids.push_back((block_id * 6) + 1);

			vertices.push_back(f_x);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z);
			tex_coords.push_back(1.f);
			tex_coords.push_back(1.f);
			block_ids.push_back((block_id * 6) + 1);

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
			block_ids.push_back((block_id * 6) + 2);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y);
			vertices.push_back(f_z);
			tex_coords.push_back(1.f);
			tex_coords.push_back(0.f);
			block_ids.push_back((block_id * 6) + 2);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z);
			tex_coords.push_back(1.f);
			tex_coords.push_back(1.f);
			block_ids.push_back((block_id * 6) + 2);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(0.f);
			tex_coords.push_back(1.f);
			block_ids.push_back((block_id * 6) + 2);

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
			block_ids.push_back((block_id * 6) + 3);

			vertices.push_back(f_x);
			vertices.push_back(f_y);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(1.f);
			tex_coords.push_back(0.f);
			block_ids.push_back((block_id * 6) + 3);

			vertices.push_back(f_x);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(1.f);
			tex_coords.push_back(1.f);
			block_ids.push_back((block_id * 6) + 3);

			vertices.push_back(f_x);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z);
			tex_coords.push_back(0.f);
			tex_coords.push_back(1.f);
			block_ids.push_back((block_id * 6) + 3);

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
			block_ids.push_back((block_id * 6) + 4);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(1.f);
			tex_coords.push_back(0.f);
			block_ids.push_back((block_id * 6) + 4);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z);
			tex_coords.push_back(1.f);
			tex_coords.push_back(1.f);
			block_ids.push_back((block_id * 6) + 4);

			vertices.push_back(f_x);
			vertices.push_back(f_y + 1.f);
			vertices.push_back(f_z);
			tex_coords.push_back(0.f);
			tex_coords.push_back(1.f);
			block_ids.push_back((block_id * 6) + 4);

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
			block_ids.push_back((block_id * 6) + 5);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y);
			vertices.push_back(f_z);
			tex_coords.push_back(1.f);
			tex_coords.push_back(0.f);
			block_ids.push_back((block_id * 6) + 5);

			vertices.push_back(f_x + 1.f);
			vertices.push_back(f_y);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(1.f);
			tex_coords.push_back(1.f);
			block_ids.push_back((block_id * 6) + 5);

			vertices.push_back(f_x);
			vertices.push_back(f_y);
			vertices.push_back(f_z + 1.f);
			tex_coords.push_back(0.f);
			tex_coords.push_back(1.f);
			block_ids.push_back((block_id * 6) + 5);

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
			{
				MakeBlock(x, y, z, GetBlock(x, y, z).m_block_id);
			}
		}
	}

	// empty
	if (indices.empty())
		return false;

	auto positions_buffer = &m_model.GetVBOs()[0];
	auto tex_coords_buffer = &m_model.GetVBOs()[1];
	auto block_id_buffer = &m_model.GetVBOs()[2];
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

	// block ids
	block_id_buffer->Bind();
	glBufferData(GL_ARRAY_BUFFER, block_ids.size() * sizeof(BLOCK_ID), block_ids.data(), GL_DYNAMIC_DRAW);
	Mango::VertexArray::EnableAttributeInt(2, 1, GL_UNSIGNED_SHORT, sizeof(BLOCK_ID), 0);

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
