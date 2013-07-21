#include "sts.h"



void sts_node_init(sts_node_t * node,
		void (* function) (void),
		time_t duration)
{
	node->function = function;
	node->duration = duration;
}

void sts_table_init(sts_table_t * table, sts_node_t * nodes, size_t size)
{
	table->nodes = nodes;
	table->size = size;
}


void sts_scheduler_init(
		sts_scheduler_t * scheduler,
		sts_table_t * table,
		time_t (*get_current_time) (void),
		void (*sleep) (void))
{
	scheduler->table = table;
	scheduler->nextTable = 0;
	scheduler->position = 0;
	scheduler->state = sts_state_READY;
	// Warning: Must not be null
	scheduler->get_current_time = get_current_time;
	// Warning: Must not be null
	scheduler->sleep = sleep;
}



void sts_scheduler_step(sts_scheduler_t * scheduler)
{
	time_t start;
	time_t taskEnd;
	time_t nodeEnd;
	sts_node_t node;

	// Get the current time to track task/node start
  start	= scheduler->get_current_time();
	// Get the node to step over
	node = scheduler->table->nodes[scheduler->position];
	// Execute the task
	if (node->function)
	{
		node->function();
	}
	// Get the current time to track task end
	taskEnd = scheduler->get_current_time();
	// Advance to next node
	scheduler->position++;
	// Check if end of table has been reached
	if (scheduler->position == scheduler->table->size)
	{
		// Check for a following table
		// Implementation note: A following table is supported to allow changing
		// the table from any node, completing only after the current table has
		// been finished.
		if (scheduler->nextTable)
		{
			// Use the next table as current, clearing the next pointer
			scheduler->table = scheduler->nextTable;
			scheduler->nextTable = 0;
		}
		else
		{
			// Check if the scheduler is about to stop, and stop in case no more
			// nodes (in this or a following table) are available.
			if (scheduler->state == sts_state_STOPPING)
			{
				scheduler->state = sts_state_STOPPED;
			}
		}
		// Reset position to begin of the table
		scheduler->position = 0;
	}
	// Compute wanted node end
	nodeEnd = start + node->duration;
	// Check if nodes duration isn't over yet
	if (endTime - startTime < duration)
	{
		// Sleep to fill node duration
		// Implementation note: Test current time here again, although previously
		// tested, to allow for fine grained decisions.
		// Sleeping first and checking afterwards may lead to inaccuracies.
		while (scheduler->get_current_time() != nodeEnd)
		{
			// Implementation note: Because test and sleep is not atomic,
			// inaccuracies are possible in case the timer interrupt fires between
			// the test and sleep operations.
			scheduler->sleep();
		}
	}
}



