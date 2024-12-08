
#include <gtest/gtest.h>

#include "core/perf/include/perf.hpp"
#include "seq/baranov_a_odd_even_mergesort/include/header_seq_odd_even.hpp"

TEST(seq_baranov_a_odd_even_sort, test_pipeline_run) {
  int number = 100000;
  std::vector<int> global_vec(number);
  std::vector<int> out(number);

  // Create TaskData
  std::random_device rd;
  std::default_random_engine reng(rd());
  std::uniform_int_distribution<int> dist(0, global_vec.size());
  std::generate(global_vec.begin(), global_vec.end(), [&dist, &reng] { return dist(reng); });
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(global_vec.data()));
  taskDataSeq->inputs_count.emplace_back(global_vec.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskSequential =
      std::make_shared<baranov_a_qsort_odd_even_merge_seq::odd_even_mergesort_seq<int>>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskSequential);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  std::sort(global_vec.begin(), global_vec.end());
  ASSERT_EQ(global_vec, out);
}

TEST(seq_baranov_a_odd_even_sort, test_task_run) {
  int number = 100000;
  std::vector<int> global_vec(number);
  std::vector<int> out(number);
  // Create TaskData
  std::random_device rd;
  std::default_random_engine reng(rd());
  std::uniform_int_distribution<int> dist(0, global_vec.size());
  std::generate(global_vec.begin(), global_vec.end(), [&dist, &reng] { return dist(reng); });
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(global_vec.data()));
  taskDataSeq->inputs_count.emplace_back(global_vec.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskSequential =
      std::make_shared<baranov_a_qsort_odd_even_merge_seq::odd_even_mergesort_seq<int>>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskSequential);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  std::sort(global_vec.begin(), global_vec.end());
  ASSERT_EQ(global_vec, out);
}