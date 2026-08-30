#pragma once

#include "../utils/ddefines.h"

typedef struct dgraph * dgraph_t;
typedef enum dgraph_type_e {
    DGRAPH_TYPE_UNDIRECTED,
    DGRAPH_TYPE_DIRECTED,
    DGRAPH_TYPE_UNDIRECTED_WEIGHTED,
    DGRAPH_TYPE_DIRECTED_WEIGHTED,
    DGRAPH_TYPE_COUNT
} dgraph_type_e;

typedef struct dgraph_mem_allocator {
    void * (*allocate)(u64);
    void * (*reallocate)(void*, u64);
    void   (*free)(void*);
} dgraph_mem_allocator;

dgraph_t dgraph_create(const dgraph_type_e dgraph_type, dgraph_mem_allocator* allocator);
void     dgraph_destroy(dgraph_t dgraph);
bool     dgraph_add_vertex(dgraph_t dgraph, u32 ID, void* user_data, u64 sizeof_userdata);
bool     dgraph_add_edge(dgraph_t dgraph, u32 inID, u32 outID, f32 weight, void* user_data, u32 sizeof_userdata);
bool     dgraph_get_vertex(dgraph_t dgraph, u32 ID, void* out_user_data);
bool     dgraph_get_edge(dgraph_t dgraph, u32 inID, u32 outID, f32 * weight, void* out_user_data);
bool     dgraph_set_edge_weight_and_data(dgraph_t dgraph, u32 inID, u32 outID, f32 weight, void* new_user_data);
bool     dgraph_set_edge_data(dgraph_t dgraph, u32 inID, u32 outID, void* new_user_data);
bool     dgraph_set_edge_weight(dgraph_t dgraph, u32 inID, u32 outID, f32 weight);
bool     dgraph_set_vertex_data(dgraph_t dgraph, u32 ID, void* new_user_data);

#if defined(DGRAPH_IMPLEMENTATION) || 1

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define DGRAPH_DEFAULT_MEM_ALLOCATOR (dgraph_mem_allocator){ .allocate = malloc, .reallocate = realloc, .free = free }

#if !defined(DGRAPH_VERTICES_INITIAL_CAPACITY) 
#   define DGRAPH_VERTICES_INITIAL_CAPACITY 64
#endif
#if !defined(DGRAPH_EDGES_INITIAL_CAPACITY) 
#   define DGRAPH_EDGES_INITIAL_CAPACITY 128
#endif

typedef struct dedge {
    u32 inID, outID;
    u32 user_data_size;
    f32 weight;
    void* user_data;
} dedge_t;

typedef struct dvertex {
    u32 ID;
    u32 user_data_size;
    void* user_data;
} dvertex_t; 

typedef struct dgraph {
    dgraph_type_e type; 
    u32 vertices_count, vertices_capacity;
    u32 edges_count, edges_capacity;
    dgraph_mem_allocator allocator;
    dvertex_t* vertices;
    dedge_t* edges;
} * dgraph_t;

dgraph_t dgraph_create(dgraph_type_e dgraph_type, dgraph_mem_allocator* _allocator)
{
    dgraph_mem_allocator allocator = _allocator == NULL ? DGRAPH_DEFAULT_MEM_ALLOCATOR : *_allocator;
    dgraph_t dgraph = (dgraph_t) allocator.allocate (sizeof(struct dgraph));
    assert(dgraph);
    
    dgraph->type              = dgraph_type;
    dgraph->allocator         = allocator;
    dgraph->edges_count       = 0;
    dgraph->vertices_count    = 0;
    dgraph->edges_capacity    = DGRAPH_EDGES_INITIAL_CAPACITY;
    dgraph->vertices_capacity = DGRAPH_VERTICES_INITIAL_CAPACITY;
    dgraph->edges             = allocator.allocate(DGRAPH_EDGES_INITIAL_CAPACITY * sizeof(dvertex_t));
    dgraph->vertices          = allocator.allocate(DGRAPH_VERTICES_INITIAL_CAPACITY * sizeof(dedge_t));
    
    return dgraph;
}

void dgraph_destroy(dgraph_t dgraph)
{
    if (dgraph)
    {
        if (dgraph->vertices) dgraph->allocator.free(dgraph->vertices);
        if (dgraph->edges) dgraph->allocator.free(dgraph->edges);
        dgraph->allocator.free(dgraph);
    }
}

bool dgraph_add_vertex(dgraph_t dgraph, u32 ID, void *user_data, u64 sizeof_userdata)
{
    // TODO: check for multiple ID ?
    if (dgraph->vertices_count >= dgraph->vertices_capacity)
    {
        dgraph->vertices_capacity *= 2;
        void* re_block = dgraph->allocator.reallocate(dgraph->vertices, dgraph->vertices_capacity * sizeof(dvertex_t));
        if (!re_block) return false;
        dgraph->vertices = re_block;
    }

    dvertex_t v = { 
        .ID             = ID,
        .user_data      = dgraph->allocator.allocate(sizeof_userdata),
        .user_data_size = sizeof_userdata,
    };
    
    memcpy(v.user_data, user_data, sizeof_userdata);
    
    dgraph->vertices[ dgraph->vertices_count++ ] = v;
    return true;
}

bool dgraph_add_edge(dgraph_t dgraph, u32 inID, u32 outID, f32 weight, void* user_data, u32 sizeof_userdata)
{
    // TODO: check for multiple ID ?
    if (dgraph->edges_count >= dgraph->edges_capacity)
    {
        dgraph->edges_capacity *= 2;
        void * re_block = dgraph->allocator.reallocate(dgraph->edges, dgraph->edges_capacity * sizeof(dedge_t));
        if (!re_block) return false;
        dgraph->edges = re_block;
    }

    dedge_t e = {
        .inID           = inID,
        .outID          = outID,
        .user_data      = dgraph->allocator.allocate(sizeof_userdata),
        .user_data_size = sizeof_userdata,
        .weight         = weight
    };

    memcpy(e.user_data, user_data, sizeof_userdata);

    dgraph->edges[ dgraph->edges_count++ ] = e;
    return true;
}

bool dgraph_get_vertex(dgraph_t dgraph, u32 ID, void *out_user_data)
{
    for (u32 i = 0; i < dgraph->vertices_count; i++)
    {
        if (dgraph->vertices[i].ID == ID)
        {
            if (out_user_data) memcpy(out_user_data, dgraph->vertices[i].user_data, dgraph->vertices[i].user_data_size);
            return true;
        }
    }
    return false;
}

bool dgraph_get_edge(dgraph_t dgraph, u32 inID, u32 outID, f32 *weight, void *out_user_data)
{
    for (u32 i = 0; i < dgraph->edges_count; i++)
    {
        dedge_t e = dgraph->edges[i];
        if (e.inID == inID && e.outID == outID)
        {
            if (out_user_data) memcpy(out_user_data, e.user_data, e.user_data_size);
            if (weight) memcpy(weight, &e.weight, sizeof(f32));
            return true;
        }
    }
    return false;
}

bool dgraph_set_edge_weight_and_data(dgraph_t dgraph, u32 inID, u32 outID, f32 weight, void *new_user_data)
{
    for (u32 i = 0; i < dgraph->edges_count; i++)
    {
        dedge_t e = dgraph->edges[i];
        if (e.inID == inID && e.outID == outID)
        {
            if (new_user_data) memcpy(e.user_data, new_user_data, e.user_data_size);
            dgraph->edges[i].weight = weight;
            return true;
        }
    }
    return false;
}

bool dgraph_set_edge_data(dgraph_t dgraph, u32 inID, u32 outID, void *new_user_data)
{
    for (u32 i = 0; i < dgraph->edges_count; i++)
    {
        dedge_t e = dgraph->edges[i];
        if (e.inID == inID && e.outID == outID)
        {
            if (new_user_data) memcpy(e.user_data, new_user_data, e.user_data_size);
            return true;
        }
    }
    return false;
}

bool dgraph_set_edge_weight(dgraph_t dgraph, u32 inID, u32 outID, f32 weight)
{
    for (u32 i = 0; i < dgraph->edges_count; i++)
    {
        dedge_t e = dgraph->edges[i];
        if (e.inID == inID && e.outID == outID)
        {
            dgraph->edges[i].weight = weight;
            return true;
        }
    }
    return false;
}

bool dgraph_set_vertex_data(dgraph_t dgraph, u32 ID, void *new_user_data)
{
    for (u32 i = 0; i < dgraph->vertices_count; i++)
    {
        if (dgraph->vertices[i].ID == ID)
        {
            if (new_user_data) memcpy(dgraph->vertices[i].user_data, new_user_data, dgraph->vertices[i].user_data_size);
            return true;
        }
    }
    return false;
}

#endif