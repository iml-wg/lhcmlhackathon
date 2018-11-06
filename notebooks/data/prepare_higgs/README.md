# Creating a ROOT file for Higgs dataset

To get the data run 
```
./get-csv.sh
```

Then split the dataset into signal and background running 

```
./extract-sig-bgk.sh
```

And finally lets to create the ROOT file executing
```
root -b -l -q csv2root.C
```

# Download prepared ROOT file

You can download a prepared ROOT file  from

https://cernbox.cern.ch/index.php/s/nNcMcVuK1kpPnEd
