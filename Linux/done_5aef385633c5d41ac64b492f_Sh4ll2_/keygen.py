def fce(rq):
    data = []
    i = 0
    while (i < rq):
        data.append(i)
        i += 0.8
    return data



for required_value in range(0,100):
    result = 0

    for i in fce(required_value):
        result += required_value + i

    print(required_value)
    print(result // 1)
    if (result // 1) == 4550:
        break;




x = fce(49)
print("Data: ", x)
print("Len: ", len(x))
print("Sum: ", sum(x))