#pragma once

#include "chunk.h"
#include "entity.h"
#include "ray_tracer.h"
#include "block_map.h"


class World
{
public:
	static bool DoesWorldExist(std::string world_name);
	static void CreateNewWorld(std::string world_name, uint32_t seed);
	static void DeleteWorld(std::string world_name);

public:
	bool Setup(Mango::MangoCore* mango_core, std::string world_name, const BlockMap& block_map);
	void Release();

	void Render(Mango::MangoCore* mango_core, float lerp);
	void Update(glm::fvec3 position);

	BlockMap& GetBlockMap() { return m_block_map; }
	void EditBlock(int x, int y, int z, const Block& block);
	bool GetBlock(int x, int y, int z, Block& block);
	bool GetBlock(double x, double y, double z, Block& block) { return GetBlock(int(x), int(y), int(z), block); }

	void SetRenderDistance(int render_distance) { m_render_distance = render_distance; m_should_reload_world = true; }
	int GetRenderDistance() const { return m_render_distance; }

	template <typename T, typename... pack>
	T* AddEntity(MangoApp* mango_app, pack... args)
	{
		auto entity = new T(args...);
		entity->SetMangoApp(mango_app);
		entity->OnInit();
		m_entities.push_back(entity);
		return entity;
	}

	const std::deque<std::shared_ptr<Chunk>>& GetRenderChunks() const { return m_render_chunks; }
	const std::unordered_map<uint64_t, std::shared_ptr<Chunk>>& GetChunks() const { return m_chunks; }
	const std::unordered_map<uint64_t, std::deque<EditedBlock>>& GetEditedBlocks() const { return m_edited_blocks; }
	RayTracer* GetRayTracer() { return &m_ray_tracer; }

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
	std::deque<Entity*> m_entities;
	BlockMap m_block_map;

	std::deque<Chunk*> m_update_chunks;
	std::deque<uint64_t> m_load_chunks;
	std::deque<std::shared_ptr<Chunk>> m_render_chunks;
	std::unordered_map<uint64_t, std::shared_ptr<Chunk>> m_chunks;
	std::unordered_map<uint64_t, std::deque<EditedBlock>> m_edited_blocks;

	int m_render_distance = 12;
	bool m_should_reload_world = false;
};