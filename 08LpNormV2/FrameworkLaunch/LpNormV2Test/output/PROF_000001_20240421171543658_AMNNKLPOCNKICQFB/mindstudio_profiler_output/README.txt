Timeline file description:
1.prof_rule.json:Here is no description about this file: prof_rule, please check in 'Profiling Instructions'!
2.msprof.json:Timeline report.
	1 CPU Layer: Data at the application layer, including the time consumption information of upper-layer application operators. The data needs to be collected only in msproftx or PyTorch scenarios.
	2 CANN Layer: Data at the CANN layer, including the time consumption data of components (such as AscendCL and Runtime) and nodes (operators).
	3 Ascend Hardware Layer: Bottom-layer NPU data, including the time consumption data and iteration trace data of each task stream under Ascend Hardware, HCCL and Overlap Analysis communication data, and other system data.
	4 Overlap Analysis Layer: In cluster or multi-device scenarios, computation and communication are sometimes parallel. You can check the pipeline overlapping time (time when computation and communication are parallel) to determine the computation and communication efficiencies.
		Communication Layer: Communication time.
		Communication(Not Overlaopped) Layer: Communication time that is not overlapped.
		Computing: Computation time.
		Free: Interval.

Summary file description:
1.task_time.csv:Task Scheduler summary.
	Waiting: Total wait time of a task (μs).
	Running: Total run time of a task (μs). An abnormally large value indicates that the operator implementation needs to be improved.
	Pending: Total pending time of a task (μs).
2.api_statistic.csv:Time spent by AscendCL API, is used to collect statistics on the time spent by API execution at the CANN layer.
	Level: Level of an API, including AscendCL, Runtime, Node, Model, and HCCL.

