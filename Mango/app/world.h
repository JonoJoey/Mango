#pragma once

#include "chunk.h"
#include "entity.h"
#include "ray_tracer.h"


class World
{
public:
	static bool DoesWorldExist(std::string world_path);
	static void CreateNewWorld(std::string world_path, uint32_t seed);

public:
	bool Setup(std::string world_path, std::unordered_map<std::string, BLOCK_ID> block_map);
	void Release();

	void Render();
	void Update(glm::fvec3 position);

	void EditBlock(int x, int y, int z, const Block& block);
	bool GetBlock(int x, int y, int z, Block& block);

	void SetRenderDistance(int render_distance) { m_render_distance = render_distance; m_should_reload_world = true; }
	int GetRenderDistance() const { return m_render_distance; }

	RayTracer* GetRayTracer() { return &m_ray_tracer; }

	const std::deque<std::shared_ptr<Chunk>>& GetRenderChunks() const { return m_render_chunks; }
	const std::unordered_map<uint64_t, std::shared_ptr<Chunk>>& GetChunks() const { return m_chunks; }
	const std::unordered_map<uint64_t, std::deque<EditedBlock>>& GetEditedBlocks() const { return m_edited_blocks; }

private:
	std::shared_ptr<Chunk> NewChunk(int x, int z) { return (m_chunks[PackChunk(x, z)] = std::shared_ptr<Chunk>(new Chunk(x, z))); }
	Chunk* GetChunk(int x, int z);
	void GenerateChunk(Chunk* chunk);
	void LoadChunk(int x, int z, Chunk* chunk);

	void UpdateEntities();

private:
	static void SaveChunk(int x, int z, std::deque<EditedBlock> edited_blocks, std::string world_path);

private:
	RayTracer m_ray_tracer;
	std::string m_world_path;
	siv::PerlinNoise m_perlin_noise;
	std::unordered_map<std::string, BLOCK_ID> m_block_map;

	std::deque<Chunk*> m_update_chunks;
	std::deque<uint64_t> m_load_chunks;
	std::deque<std::shared_ptr<Chunk>> m_render_chunks;
	std::unordered_map<uint64_t, std::shared_ptr<Chunk>> m_chunks;
	std::unordered_map<uint64_t, std::deque<EditedBlock>> m_edited_blocks;

	int m_render_distance = 12;
	bool m_should_reload_world = false;
};