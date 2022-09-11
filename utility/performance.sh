perf record --call-graph lbr -- ./bin/led
perf report -g graph --no-children
