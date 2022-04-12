从百度Apollo项目看算法软件架构{#cpp_apollo_arch}
============================================

<hr>
# 一个增加算法模块的实例

https://github.com/ApolloAuto/apollo/blob/$1/docs/howto/how_to_add_a_new_predictor_in_prediction_module.md

总结一下，

1. 使用虚类定义接口

2. 使用prediction_conf.proto增加派生的predictor到枚举中。

3. 在prediction_conf.pb.txt中更新predictor_type字段。

4. Register 派生类。
