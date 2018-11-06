#Author: Omar.Zapata@cern.ch http://oproject.org
#!/bin/bash

#NOTES: the firts colunm is sig/bgk where 1 is signal.

#extract signal
cat HIGGS.csv | grep "^1.000000000000000000e+00" | gawk -F"," '{print $2","$3","$4","$5","$6","$7","$8","$9","$10","$11","$12","$13","$14","$15","$16","$17","$18","$19","$20","$21","$22","$23","$24","$25","$26","$27","$28","$29}' > higgs-signal.csv

#extract background
cat HIGGS.csv | grep "^0.000000000000000000e+00" | gawk -F"," '{print $2","$3","$4","$5","$6","$7","$8","$9","$10","$11","$12","$13","$14","$15","$16","$17","$18","$19","$20","$21","$22","$23","$24","$25","$26","$27","$28","$29}' > higgs-bgk.csv

