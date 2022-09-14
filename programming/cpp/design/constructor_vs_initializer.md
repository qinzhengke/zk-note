# 构造函数（constructor） vs 初始化函数（initialization）


【问题】在实际OOP程序开发过程中，常常遇到的模型就是一个Object到底应该使用构造函数来完成初始化，还是设立额外的构造函数？

自己在思考这个问题的时候，总是迷迷糊糊，后来看了StackOverflow的一个相关讨论，解惑了一些。

【来源】StackOverflow提问：“Why use an initialization method instead of a constructor?”

提问者提出我问题的原因是因为公司同事要求使用initialization函数进行初始化，但是却不理解里面的道理。

前几个回答都比较偏向使用构造函数，认为initialization是一种“old school”的方法。

但是initailization有一些独特的能力：

1. 能够使用虚函数，不同的实例实际执行的初始化函数不一样。
2. 可以使用返回值来判断初始化是否成功，避免使用exception，后者为啥要避免也是有争议的话题。