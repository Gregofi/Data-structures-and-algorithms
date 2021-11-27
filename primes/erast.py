



def erast(P):
    seen = (P + 1)*[0]
    seen[1] = 1
    res = []
    for i in range(2, P+1):
        if seen[i] == 0:
            res.append(i)
            for j in range(i*i, P+1, i):
                seen[j] = 1
    return res

if __name__ == "__main__":
    assert erast(8) == [2, 3, 5, 7]
    assert erast(6) == [2, 3, 5]
    assert erast(1) == []
    assert erast(20) == [2, 3, 5, 7, 11, 13, 17, 19]
