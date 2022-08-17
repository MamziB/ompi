/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011-2013 Sandia National Laboratories.  All rights reserved.
 * Copyright (c) 2015      Los Alamos National Security, LLC.  All rights
 *                         reserved.
 * Copyright (C) Mellanox Technologies Ltd. 2001-2017. ALL RIGHTS RESERVED.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#ifndef OMPI_OSC_UCX_REQUEST_H
#define OMPI_OSC_UCX_REQUEST_H

#include "ompi/request/request.h"


enum acc_phases {
    ACC_INIT,
    ACC_GET_RESULTS_DATA,
    ACC_GET_STAGE_DATA,
    ACC_PUT_TARGET_DATA
};

typedef struct ompi_osc_ucx_accumulate_request {
    bool is_accumulate;
    struct ompi_op_t *op;
    int phase;
    bool lock_acquired;
    ompi_osc_ucx_module_t *module;
    int target;
    struct ompi_win_t *win;
    const void *origin_addr;
    int origin_count;
    struct ompi_datatype_t *origin_dt;
    void *stage_addr;
    int stage_count;
    struct ompi_datatype_t *stage_dt;
    struct ompi_datatype_t *target_dt;
    int target_disp;
    int target_count;
} ompi_osc_ucx_accumulate_request_t;

typedef struct ompi_osc_ucx_request {
    ompi_request_t super;
    ompi_osc_ucx_accumulate_request_t acc;
} ompi_osc_ucx_request_t;

OBJ_CLASS_DECLARATION(ompi_osc_ucx_request_t);

#define OMPI_OSC_UCX_REQUEST_ALLOC(win, req)                            \
    do {                                                                \
        opal_free_list_item_t *item;                                    \
        do {                                                            \
            item = opal_free_list_get(&mca_osc_ucx_component.requests); \
            if (item == NULL) {                                         \
                if (mca_osc_ucx_component.num_incomplete_req_ops > 0) { \
                    opal_common_ucx_wpool_progress(mca_osc_ucx_component.wpool); \
                }                                                       \
            }                                                           \
        } while (item == NULL);                                         \
        req = (ompi_osc_ucx_request_t*) item;                           \
        OMPI_REQUEST_INIT(&req->super, false);                          \
        req->super.req_mpi_object.win = win;                            \
        req->super.req_complete = false;                                \
        req->super.req_state = OMPI_REQUEST_ACTIVE;                     \
        req->super.req_status.MPI_ERROR = MPI_SUCCESS;                  \
        req->acc.op = MPI_NO_OP;                                        \
        req->acc.phase = ACC_INIT;                                      \
        req->acc.is_accumulate = false;                                 \
        req->acc.module = NULL;                                         \
        req->acc.target = -1;                                           \
        req->acc.lock_acquired = false;                                 \
        req->acc.win = NULL;                                            \
        req->acc.origin_addr = NULL;                                    \
        req->acc.origin_count = 0;                                      \
        req->acc.origin_dt = NULL;                                      \
        req->acc.stage_addr = NULL;                                     \
        req->acc.stage_count = 0;                                       \
        req->acc.stage_dt = NULL;                                       \
        req->acc.target_dt = NULL;                                      \
        req->acc.target_count = 0;                                      \
        req->acc.target_disp = 0;                                       \
    } while (0)

#define OMPI_OSC_UCX_REQUEST_RETURN(req)                                \
    do {                                                                \
        OMPI_REQUEST_FINI(&req->super);                                 \
        opal_free_list_return (&mca_osc_ucx_component.requests,         \
                               (opal_free_list_item_t*) req);           \
    } while (0)

void req_completion(void *request);

#endif /* OMPI_OSC_UCX_REQUEST_H */
