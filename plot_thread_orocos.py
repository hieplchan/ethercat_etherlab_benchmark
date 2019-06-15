import matplotlib.pyplot as plt
from matplotlib import colors
import csv
import numpy as np

num = []
period = []
latency = []
executive = []
error = 0

N_point = 0
n_bins = 100

################# CALCULATE RESULT #################
with open('histogram_thread_orocos.csv','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        num.append(int(row[0]))
        period.append(int(row[1]))
        latency.append(int(row[2]))
        executive.append(int(row[3]))
        error = int(row[4])
        N_point = N_point + 1

period_max = max(period)
period_min = min(period)
period_avr = np.mean(period)
period_std = np.std(period)

latency_max = max(latency)
latency_min = min(latency)
latency_avr = np.mean(latency)
latency_std = np.std(latency)

executive_max = max(executive)
executive_min = min(executive)
executive_avr = np.mean(executive)
executive_std = np.std(executive)

################# SAVE RESULT #################
result = open('output/thread_orocos/result.txt','w')
result.write('Result: Max - Min - Avr - Standard Deviation\n')
result.write('Period: ' + str(period_max) + ' - '  + str(period_min) + ' - ' +str(period_avr) + ' - ' + str(period_std) + '\n')
result.write('Latency: ' + str(latency_max) + ' - '  + str(latency_min) + ' - ' +str(latency_avr) + ' - ' + str(latency_std) + '\n')
result.write('Executive: ' + str(executive_max) + ' - '  + str(executive_min) + ' - ' +str(executive_avr) + ' - ' +str(executive_std) + '\n')
result.write('Error: ' + str(error))
result.close()

################# PLOT HISTOGRAM #################
N, bins, patches = plt.hist(period, bins=n_bins)
fracs = N.astype(float) / N.max()
norm = colors.Normalize(fracs.min(), fracs.max())
for thisfrac, thispatch in zip(fracs, patches):
    color = plt.cm.viridis(norm(thisfrac))
    thispatch.set_facecolor(color)
plt.xlabel('period [ns] - Max: ' + str(period_max) + ' - Avr: ' + str(period_avr) + ' - Standard Deviation: ' +str(period_std))
plt.ylabel('number of sample')
plt.title('Period Histogram - Thread - Orocos')
plt.legend()
plt.savefig('output/thread_orocos/his_period.jpg', transparent=False, bbox_inches='tight', pad_inches=0)
plt.show()

N, bins, patches = plt.hist(latency, bins=n_bins)
fracs = N.astype(float) / N.max()
norm = colors.Normalize(fracs.min(), fracs.max())
for thisfrac, thispatch in zip(fracs, patches):
    color = plt.cm.viridis(norm(thisfrac))
    thispatch.set_facecolor(color)
plt.xlabel('latency [ns] - Max: '+ str(latency_max) + ' - Avr: ' + str(latency_avr) + ' - Standard Deviation: ' +str(latency_std))
plt.ylabel('number of sample')
plt.title('Latency Histogram - Thread - Orocos')
plt.legend()
plt.savefig('output/thread_orocos/his_latency.jpg', transparent=False, bbox_inches='tight', pad_inches=0)
plt.show()

N, bins, patches = plt.hist(executive, bins=n_bins)
fracs = N.astype(float) / N.max()
norm = colors.Normalize(fracs.min(), fracs.max())
for thisfrac, thispatch in zip(fracs, patches):
    color = plt.cm.viridis(norm(thisfrac))
    thispatch.set_facecolor(color)
plt.xlabel('executive [ns] - Max: '+ str(executive_max) + ' - Avr: ' + str(executive_avr) + ' - Standard Deviation: ' +str(executive_std))
plt.ylabel('number of sample')
plt.title('Executive Histogram - Thread - Orocos')
plt.legend()
plt.savefig('output/thread_orocos/his_executive.jpg', transparent=False, bbox_inches='tight', pad_inches=0)
plt.show()

################# PLOT TIME #################
plt.plot(num,period,label='Period - Thread - Orocos')
plt.xlabel('sample - Max: ' + str(period_max) + ' - Avr: ' + str(period_avr) + ' - Standard Deviation: ' +str(period_std))
plt.ylabel('period [ns]')
plt.title('Period')
plt.legend()
plt.savefig('output/thread_orocos/plot_period.jpg', transparent=False, bbox_inches='tight', pad_inches=0)
plt.show()

plt.plot(num,latency,label='Latency - Thread - Orocos')
plt.xlabel('sample - Max: '+ str(latency_max) + ' - Avr: ' + str(latency_avr) + ' - Standard Deviation: ' +str(latency_std))
plt.ylabel('latency [ns]')
plt.title('Latency')
plt.legend()
plt.savefig('output/thread_orocos/plot_latency.jpg', transparent=False, bbox_inches='tight', pad_inches=0)
plt.show()

plt.plot(num,executive,label='Executive - Thread - Orocos')
plt.xlabel('sample - Max: '+ str(executive_max) + ' - Avr: ' + str(executive_avr) + ' - Standard Deviation: ' +str(executive_std))
plt.ylabel('executive [ns]')
plt.title('Executive')
plt.legend()
plt.savefig('output/thread_orocos/plot_executive.jpg', transparent=False, bbox_inches='tight', pad_inches=0)
plt.show()
