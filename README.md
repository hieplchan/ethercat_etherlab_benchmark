# EtherLab EtherCAT master Test & Benchmark with Orocos 7 DOF Robot Arm Inverse Dynamics

# 1. EtherLab Master
## 1.1 Start EtherLab master
```
sudo /etc/init.d/ethercat restart
/opt/etherlab/bin/ethercat master
/opt/etherlab/bin/ethercat slaves -v
```

## 1.2 Test performance no thread
```
gcc dc_test_no_thread.c -o dc_test_no_thread -I/opt/etherlab/include -L/opt/etherlab/lib -lethercat -Wl,--rpath -Wl,/opt/etherlab/lib
sudo ./dc_test_no_thread > histogram_no_thread.csv
python plot_no_thread.py
```

## 1.3 Test performance thread
```
gcc dc_test_thread.c -lpthread -o dc_test_thread -I/opt/etherlab/include -L/opt/etherlab/lib -lethercat -Wl,--rpath -Wl,/opt/etherlab/lib
sudo ./dc_test_thread > histogram_thread.csv
python plot_thread.py
```

# 2. EtherLab Master
## 2.1 Test performance thread + inverse dynamics 7 DOF robot arm
```
source /home/hiep/linuxcnc-dev/scripts/rip-environment
g++ -c -fPIC -L/usr/local/lib -I. -I/usr/local/include -I/usr/include/eigen3 -lorocos-kdl orocos_cnc.cpp -o orocos_cnc.o
g++ -fPIC -shared orocos_cnc.o -o liborocos_cnc.so

gcc dc_test_thread_orocos.c -lpthread -o dc_test_thread_orocos -I/opt/etherlab/include -L/opt/etherlab/lib -L. -lethercat -lorocos_cnc -lorocos-kdl -Wl,-rpath,/opt/etherlab/lib -Wl,-rpath,/home/hiep/hiep_igh/thread_id7dof
sudo ./dc_test_thread_orocos > ../histogram_thread_orocos.csv
python plot_thread_orocos.py
```

## 2.2 LinuxCNC thread + inverse dynamics 7 DOF robot arm
```
gcc etherlab_thread_orocos.c -lpthread -o etherlab_thread_orocos -I. -I/opt/etherlab/include -L/opt/etherlab/lib -L. -lethercat -lorocos_cnc -lorocos-kdl -Wl,-rpath,/opt/etherlab/lib -Wl,-rpath,/home/hiep/hiep_igh/thread_id7dof

sudo ./etherlab_thread_orocos
```
