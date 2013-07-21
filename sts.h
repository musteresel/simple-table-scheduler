/** Simple table scheduler
 *
 * Needs a sts_cfg.h file, with at least:
 *  - typedef <some_integral_type> time_t;
 * */
#ifndef ___SCHEDULER___
#define ___SCHEDULER___ "simple-table-scheduler"




//-----------------------------------------------------------------------------
#include <stdint.h>
#include "sts_cfg.h"




//-----------------------------------------------------------------------------
/** Scheduling node describing the task to run and the wanted duration of this
 * node.
 *
 * One must assure that the task does not run longer than the wanted duration,
 * because the scheduler won't enforce timings.
 * */
typedef struct __sts_node_t
{
	void (* function) (void);
	time_t duration;
} sts_node_t;




/** Scheduling table describing a schedule of multiple tasks.
 *
 * The table consits of a array of nodes, each describing a task and a
 * duration.
 * */
typedef struct __sts_table_t
{
	sts_node_t * nodes;
	size_t size;
} sts_table_t;




/** State of a scheduler used to control scheduler operation.
 * */
typedef enum __sts_state_t
{
	sts_state_STOPPED,
	sts_state_STOPPING,
	sts_state_READY,
} sts_state_t;




/** Scheduler with a table and the current position in that table.
 *
 * The scheduler may have set a nextTable, which will be used on completion
 * of the current table.
 * */
typedef struct __sts_scheduler_t
{
	sts_table_t * table;
	sts_table_t * nextTable;
	size_t position;
	sts_state_t state;
	time_t (*get_current_time) (void);
	void (*sleep) (void);
} sts_scheduler_t;




//-----------------------------------------------------------------------------
#endif
