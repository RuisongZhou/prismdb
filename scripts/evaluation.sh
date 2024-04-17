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
echo "Experiment 1 done"