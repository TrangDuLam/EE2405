# Markdown HW01

## Input stage

  Using the csv package to load the nominated data. To capture the header of the category, I set am empty list to record the all the categories into the given list.
  Since the data are preserved in the python dictionary type, I declare another matrix to capture the numerical data given the humidity category. After that, I changed the humidity list into np.array datatype.

## Processing stage

  ### (1) Data deletion

    Using the if-statement to select the data. And using the np.delete() command to delete the certain row

  ### (2) Summation

    Initialize an list to record the certain names [C0A880, C0F9A0, C0G640, C0R190, C0X260]. Then, I set up 5 arrays to capture the data respectively.
    Then, using the np.sum() command to compute the summation for elements

## Output result

[['C0A880' '21.970000000000002']

 ['C0F9A0' '21.28']

 ['C0G640' '20.04']

 ['C0R190' '18.51']
 
 ['C0X260' '20.51']]