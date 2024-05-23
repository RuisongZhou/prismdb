echo "Running Experiment 1"
sudo python3 run.py config_exp1.yml my_ycsb_log_exp1
sed -i 's/workload: \[\"ycsba/workload: \[\"ycsbb/' config_exp1.yml
sudo python3 run.py config_exp1.yml my_ycsb_log_exp1
sed -i 's/workload: \[\"ycsbb/workload: \[\"ycsbc/' config_exp1.yml
sudo python3 run.py config_exp1.yml my_ycsb_log_exp1
sed -i 's/workload: \[\"ycsbc/workload: \[\"ycsbd/' config_exp1.yml
sudo python3 run.py config_exp1.yml my_ycsb_log_exp1
sed -i 's/workload: \[\"ycsbd/workload: \[\"ycsbe/' config_exp1.yml
sudo python3 run.py config_exp1.yml my_ycsb_log_exp1
sed -i 's/workload: \[\"ycsbe/workload: \[\"ycsbf/' config_exp1.yml
sudo python3 run.py config_exp1.yml my_ycsb_log_exp1

sed -i 's/workload: \[\"ycsbf/workload: \[\"ycsba/' config_exp1.yml
echo "Experiment 1 done"

echo "Running Experiment 2"
# 纯写
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0 -1)/(0.4 -1)/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.4 -1)/(0.6 -1)/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.6 -1)/(0.8 -1)/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.8 -1)/(0.99 -1)/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.99 -1)/(1.2 -1)/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(1.2 -1)/(0 -1)/' config_exp2.yml
# 纯读
sed -i 's/read_ratio: \[0\]/read_ratio: \[1\]/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0 -1)/(0.4 -1)/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.4 -1)/(0.6 -1)/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.6 -1)/(0.8 -1)/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.8 -1)/(0.99 -1)/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.99 -1)/(1.2 -1)/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
# mix
sed -i 's/read_ratio: \[1\]/read_ratio: \[0.5\]/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0 -1)/(0.4 -1)/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.4 -1)/(0.6 -1)/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.6 -1)/(0.8 -1)/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.8 -1)/(0.99 -1)/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.99 -1)/(1.2 -1)/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2

sed -i 's/(1.2 -1)/(0 -1)/' config_exp2.yml
sed -i 's/read_ratio: \[0.5\]/read_ratio: \[0\]/' config_exp2.yml
echo "Experiment 2 done"

echo "Running Experiment 3"
sed -i 's/value_size: \[16\]/value_size: \[32\]/' config_exp3.yml
sudo python3 run.py config_exp3.yml my_ycsb_log_exp3

sed -i 's/value_size: \[32\]/value_size: \[64\]/' config_exp3.yml
sed -i 's/db_size: \[3200000000\]/db_size: \[1600000000\]/' config_exp3.yml
sudo python3 run.py config_exp3.yml my_ycsb_log_exp3

sed -i 's/value_size: \[64\]/value_size: \[128\]/' config_exp3.yml
sed -i 's/db_size: \[1600000000\]/db_size: \[800000000\]/' config_exp3.yml
sudo python3 run.py config_exp3.yml my_ycsb_log_exp3

sed -i 's/value_size: \[128\]/value_size: \[256\]/' config_exp3.yml
sed -i 's/db_size: \[800000000\]/db_size: \[400000000\]/' config_exp3.yml
sudo python3 run.py config_exp3.yml my_ycsb_log_exp3

sed -i 's/value_size: \[256\]/value_size: \[512\]/' config_exp3.yml
sed -i 's/db_size: \[400000000\]/db_size: \[200000000\]/' config_exp3.yml
sudo python3 run.py config_exp3.yml my_ycsb_log_exp3

sed -i 's/value_size: \[512\]/value_size: \[980\]/' config_exp3.yml
sed -i 's/db_size: \[200000000\]/db_size: \[100000000\]/' config_exp3.yml
sudo python3 run.py config_exp3.yml my_ycsb_log_exp3

sed -i 's/value_size: \[980\]/value_size: \[16\]/' config_exp3.yml
sed -i 's/db_size: \[100000000\]/db_size: \[3200000000\]/' config_exp3.yml
echo "Experiment 3 done"

echo "Running Experiment 4"
sudo python3 run.py config_exp4.yml my_ycsb_log_exp4
sed -i 's/optane_ratio: \[0.01\]/optane_ratio: \[0.02\]/' config_exp4.yml
sudo python3 run.py config_exp4.yml my_ycsb_log_exp4
sed -i 's/optane_ratio: \[0.02\]/optane_ratio: \[0.04\]/' config_exp4.yml
sudo python3 run.py config_exp4.yml my_ycsb_log_exp4
sed -i 's/optane_ratio: \[0.04\]/optane_ratio: \[0.08\]/' config_exp4.yml
sudo python3 run.py config_exp4.yml my_ycsb_log_exp4
sed -i 's/optane_ratio: \[0.08\]/optane_ratio: \[0.16\]/' config_exp4.yml
sudo python3 run.py config_exp4.yml my_ycsb_log_exp4

sed -i 's/optane_ratio: \[0.16\]/optane_ratio: \[0.01\]/' config_exp4.yml
echo "Experiment 4 done"
