sudo python3 run.py config_test_partition_1.yml my_ycsb_log
echo "1 done"
sudo python3 run.py config_test_partition_2.yml my_ycsb_log
echo "2 done"
sudo python3 run.py config_test_partition_4.yml my_ycsb_log
echo "4 done"
sudo python3 run.py config_test_partition_8.yml my_ycsb_log
echo "8 done"
sudo python3 run.py config_test_partition_16.yml my_ycsb_log
echo "16 done"

# sudo python3 run.py config_test_v980.yml my_ycsb_log
# echo "980 done"