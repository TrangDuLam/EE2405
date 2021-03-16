import csv
import numpy as np

def find_sum(tar, data) :

    sum = list()
    s = 0

    for item in tar :
        ds = data[data[:, 0] == item]
        s = np.sum(ds[:, 1])
        sum.append(s)

    return sum


def main() :
  cwb_filename = r'D:\EE2405-1\hw1\106061121.csv'
  data_id = []
  data_hud = []
  header = []


  with open(cwb_filename, newline='') as csvfile:
    mycsv = csv.DictReader(csvfile)
    header = mycsv.fieldnames
    for row in mycsv :
        data_id.append(row['station_id'])
        data_hud.append(row['HUMD'])

  

  data_id = np.array(data_id)
  data_hud = np.array(data_hud)
  data_hud = data_hud.astype(np.float)  #little detail to figure out
  data = np.column_stack((data_id, data_hud.astype(np.object)))

  #delete data
  for i in data[:, 1] :
     if i == -99.000 or i == -999.000 :
        i = 0

  tar = ['C0A880','C0F9A0','C0G640','C0R190','C0X260']
  sum_r = find_sum(tar, data)
  result = np.column_stack((tar, sum_r))
  print(result)

main()


