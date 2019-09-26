# Authors:
# Mariaelena D'Errico 
# Stefano Calì 

# A multilayer perceptron neural network to classify Higgs and Background events
# 3 hidden layers each composed of 10 neurons 
import csv
import numpy as np
import pylab as pl
import sklearn
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
from sklearn.neural_network import MLPClassifier
import matplotlib.pyplot as plt
import statistics as st
from sklearn import metrics

HiddenLayers = [10,10, 10]
data_class = []
data_features = []

# loading data, previously saved in .npy files 

data_class = np.load('data_classHiggs.npy')
print('class data loaded')
data_features = np.load('data_featuresHiggs.npy')
print('features data loaded')


# Reading data and save in a .npy file to get faster in further executions

'''
with open("HIGGS2.csv", newline="", encoding="ISO-8859-1") as filecsv:
    dataHiggs = csv.reader(filecsv,delimiter=",")

    list = []
    for row in dataHiggs:
        data_class.append(float(row[0]))
       
        data_features.append(row[22:28])
print('class data created')

np.save('data_classHiggs_moreVar2',data_class)

for i in range(len(data_features)):
    for j in range(len(data_features[i])):
                data_features[i][j] = float(data_features[i][j])
print('features data created')
np.save('data_featuresHiggs_moreVar2',data_features)
'''

# dataset normalization 

ratio_train = 0.7
ratio_test = 0.3
features_train, features_test, class_train, class_test = train_test_split(data_features,data_class, train_size=ratio_train,test_size=ratio_test, shuffle=True)
scaler = StandardScaler()
scaler.fit(features_train)
features_train_scale = scaler.transform(features_train)
features_test_scale = scaler.transform(features_test)

print('data scaled')


#Multilayer Perceptron train 
mpl_BP = MLPClassifier(hidden_layer_sizes=(HiddenLayers), max_iter=10000)
mpl_BP.fit(features_train_scale,class_train)
print('network fitted')

fpr2, tpr2, threshold = metrics.roc_curve(class_test, mpl_BP.predict_proba(features_test_scale)[:,1])

print('ROC curve computed')

for i in range(len(fpr2)): fpr2[i] = 1 - fpr2[i]
roc_auc2 = metrics.auc(tpr2,fpr2)
print('ROC integral= ' + str(roc_auc2))

# image drawing
plt.figure()
plt.title('ROC curve')
plt.plot(tpr2, fpr2, label = 'MLP AUC = %0.2f' % roc_auc2)
plt.legend(loc = 'lower right')
plt.plot([0, 1], [0, 1],'r--')
plt.xlim([0, 1])
plt.ylim([0, 1])
plt.ylabel('Background rejection')
plt.xlabel('Signal efficiency')
plt.show()
