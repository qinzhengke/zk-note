# JSON

## 如何将一个class变为可序列化的（serializable）？

```{python}
class Param:
def __init__(self):
    self.gt_path = ''
    self.dt_path = ''
    self.output_path = ''

with open('output.json', 'w') as ofs:
    json.dumps(Param().__dict__, ofs)
```
