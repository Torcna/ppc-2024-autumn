
#include <gtest/gtest.h>


#include "mpi/baranov_a_num_of_orderly_violations/src/source.cpp"
#include <random>
TEST(baranov_a_num_of_orderly_violations_mpi, Test_viol_100_int) {
  const int N = 100;
  // Create data
  boost::mpi::communicator world;
  
  
  std::vector<int> arr(N);
  std::vector<int> out(1);
  
  std::shared_ptr<ppc::core::TaskData> data_seq = std::make_shared<ppc::core::TaskData>();
  if (world.rank() == 0)
  {
    std::random_device rd;
    std::default_random_engine reng(rd());
    std::uniform_int_distribution<int> dist(0, arr.size());
    std::generate(arr.begin(), arr.end(), [&dist, &reng] { return dist(reng); });

    data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(arr.data()));
    data_seq->inputs_count.emplace_back(arr.size());
    data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
    data_seq->outputs_count.emplace_back(1);

  }
  
  baranov_a_num_of_orderly_violations_mpi::num_of_orderly_violations<int, int> test1(data_seq);

  ASSERT_EQ(test1.validation(), true);
  test1.pre_processing();
  test1.run();
  test1.post_processing();
  int num = test1.seq_proc(arr);
  
  ASSERT_EQ(out[0],num );

}

TEST(baranov_a_num_of_orderly_violations_mpi, Test_viol_10000_int) {
  const int N = 10000;
  // Create data
  boost::mpi::communicator world;

  std::vector<int> arr(N);
  std::vector<int> out(1);

  std::shared_ptr<ppc::core::TaskData> data_seq = std::make_shared<ppc::core::TaskData>();
  if (world.rank() == 0) {

    std::random_device rd;
    std::default_random_engine reng(rd());
    std::uniform_int_distribution<int> dist(0, arr.size());
    std::generate(arr.begin(), arr.end(), [&dist, &reng] { return dist(reng); });
    data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(arr.data()));
    data_seq->inputs_count.emplace_back(arr.size());
    data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
    data_seq->outputs_count.emplace_back(1);
  }

  baranov_a_num_of_orderly_violations_mpi::num_of_orderly_violations<int, int> test1(data_seq);

  ASSERT_EQ(test1.validation(), true);
  test1.pre_processing();
  test1.run();
  test1.post_processing();

  int num = test1.seq_proc(arr);
  
  ASSERT_EQ(num, out[0]);


}

TEST(baranov_a_num_of_orderly_violations_mpi, Test_viol_10000_double) {
  const int N = 10000;
  // Create data
  boost::mpi::communicator world;

  std::vector<double> arr(N);
  std::vector<int> out(1);

  std::shared_ptr<ppc::core::TaskData> data_seq = std::make_shared<ppc::core::TaskData>();
  if (world.rank() == 0) {

    std::random_device rd;
    std::default_random_engine reng(rd());
    std::uniform_int_distribution<int> dist(0, arr.size());
    std::generate(arr.begin(), arr.end(), [&dist, &reng] { return dist(reng); });
    data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(arr.data()));
    data_seq->inputs_count.emplace_back(arr.size());
    data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
    data_seq->outputs_count.emplace_back(1);
  }

  baranov_a_num_of_orderly_violations_mpi::num_of_orderly_violations<double, int> test1(data_seq);

  ASSERT_EQ(test1.validation(), true);
  test1.pre_processing();
  test1.run();
  test1.post_processing();

  int num = test1.seq_proc(arr);

  ASSERT_EQ(num, out[0]);

}


TEST(baranov_a_num_of_orderly_violations_mpi, Test_viol_size_of_vec_is_equal_to_world_size) {
  const int N = 3;
  // Create data
  boost::mpi::communicator world;

  std::vector<double> arr(N);
  std::vector<int> out(1);

  std::shared_ptr<ppc::core::TaskData> data_seq = std::make_shared<ppc::core::TaskData>();
  if (world.rank() == 0) {

    std::random_device rd;
    std::default_random_engine reng(rd());
    std::uniform_int_distribution<int> dist(0, arr.size());
    std::generate(arr.begin(), arr.end(), [&dist, &reng] { return dist(reng); });
    data_seq->inputs.emplace_back(reinterpret_cast<uint8_t *>(arr.data()));
    data_seq->inputs_count.emplace_back(arr.size());
    data_seq->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
    data_seq->outputs_count.emplace_back(1);
  }

  baranov_a_num_of_orderly_violations_mpi::num_of_orderly_violations<double, int> test1(data_seq);

  ASSERT_EQ(test1.validation(), true);
  test1.pre_processing();
  test1.run();
  test1.post_processing();
  
  int num = test1.seq_proc(arr);
  
  ASSERT_EQ(num, out[0]);
}


int main(int argc, char** argv) {
  boost::mpi::environment env(argc, argv);
  boost::mpi::communicator world;
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
  if (world.rank() != 0) {
    delete listeners.Release(listeners.default_result_printer());
  }
  return RUN_ALL_TESTS();
}
