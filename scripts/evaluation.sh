echo "Running Experiment 1"
sudo python3 run.py config_exp1.yml my_ycsb_log_exp1
sed -i 's/ycsba/ycsbb/' config_exp1.yml
sudo python3 run.py config_exp1.yml my_ycsb_log_exp1
sed -i 's/ycsbb/ycsbc/' config_exp1.yml
sudo python3 run.py config_exp1.yml my_ycsb_log_exp1
sed -i 's/ycsbc/ycsbd/' config_exp1.yml
sudo python3 run.py config_exp1.yml my_ycsb_log_exp1
sed -i 's/ycsbd/ycsbe/' config_exp1.yml
sudo python3 run.py config_exp1.yml my_ycsb_log_exp1
sed -i 's/ycsbe/ycsbf/' config_exp1.yml
sudo python3 run.py config_exp1.yml my_ycsb_log_exp1

sed -i 's/ycsbf/ycsba/' config_exp1.yml
echo "Experiment 1 done"

echo "Running Experiment 2"
# 纯写
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0/(0.4/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.4/(0.6/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.6/(0.8/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.8/(0.99/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.99/(1.2/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(1.2/(0/' config_exp2.yml
# 纯读
sed -i 's/read_ratio: \[0\]/read_ratio: \[1\]/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0/(0.4/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.4/(0.6/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.6/(0.8/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.8/(0.99/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2
sed -i 's/(0.99/(1.2/' config_exp2.yml
sudo python3 run.py config_exp2.yml my_ycsb_log_exp2

sed -i 's/(1.2/(0/' config_exp2.yml
sed -i 's/read_ratio: \[1\]/read_ratio: \[0\]/' config_exp2.yml
echo "Experiment 2 done"

echo "Running Experiment 3"
sudo python3 run.py config_exp3.yml my_ycsb_log_exp3
sed -i 's/16/32/' config_exp3.yml
sudo python3 run.py config_exp3.yml my_ycsb_log_exp3
sed -i 's/32/64/' config_exp3.yml
sudo python3 run.py config_exp3.yml my_ycsb_log_exp3
sed -i 's/64/128/' config_exp3.yml
sudo python3 run.py config_exp3.yml my_ycsb_log_exp3
sed -i 's/128/256/' config_exp3.yml
sudo python3 run.py config_exp3.yml my_ycsb_log_exp3
sed -i 's/256/512/' config_exp3.yml
sudo python3 run.py config_exp3.yml my_ycsb_log_exp3
sed -i 's/512/980/' config_exp3.yml
sudo python3 run.py config_exp3.yml my_ycsb_log_exp3

sed -i 's/980/16/' config_exp3.yml
echo "Experiment 3 done"

echo "Running Experiment 4"
sudo python3 run.py config_exp4.yml my_ycsb_log_exp4
sed -i 's/0.01/0.02/' config_exp4.yml
sudo python3 run.py config_exp4.yml my_ycsb_log_exp4
sed -i 's/0.02/0.04/' config_exp4.yml
sudo python3 run.py config_exp4.yml my_ycsb_log_exp4
sed -i 's/0.04/0.08/' config_exp4.yml
sudo python3 run.py config_exp4.yml my_ycsb_log_exp4
sed -i 's/0.08/0.16/' config_exp4.yml
sudo python3 run.py config_exp4.yml my_ycsb_log_exp4

sed -i 's/0.16/0.01/' config_exp4.yml
echo "Experiment 4 done"
