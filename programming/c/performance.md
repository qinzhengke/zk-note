C语言性能相关topic{#c_performance}
===============================

<hr>
\section Debug版本对performance影响很大

不要使用Debug版本进行performance相关测评。

这里使用图像下采样做了一个实验，Debug版本和Relase版本相比，慢了不止一点。

\code{cpp}
uint32_t W=640, H=480, WH=W*H;
FILE *f = fopen("img","rb");
uint8_t* buf = malloc(WH);
fread(buf, WH, 1, f);

int64_t t = cv::getTickCount();
uint8_t * img = (uint8_t*) malloc(WH);
memcpy(img, buf, WH);
t = cv::getTickCount() - t;
cout<<t/cv::getTickFrenquency()<<endl;

t = cv::getTickCount();
uint32_t sd=5, Wd=W/sd, Hd=H/sd;
uint8_t *img_d = (uint8_t*)malloc(Wd*Hd);
for(uint32_t r=0; r<Hd; r++)
  for(uint32_t c=0; c<Wd; c++)
    img_d[r*Wd+c] = buf[r*sd*W+c*sd];
t = cv::getTickCount()-t;
cout<<t/cv::getTickFrequency()<<endl;
\endcode
PC上运行的结果，t1是全拷贝的时间，t2是下采样拷贝的时间，可以看到在debug版本中，2倍采样和3倍采样时间都要长于全采样，直到4倍采样开始时间才小于全采样，这是因为下采样需要设计循环，依次赋值，全拷贝肯定在汇编层面做了优化的。
有意思的是在Release版本中，2倍采样的时间就小于全拷贝，这就说明debug版本对于循环有着更深的影响，下采样最大运行时间相差了接近6倍。


测试结果如下：时间单位为ms。

| 版本 \\ 采样率   | 1    | 2    | 3    | 4    | 5    | 6    |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| Debug   | 840  | 219  | 102  | 60.2 | 39.1 | 27.6 |
| Release   | 158  | 62.6 | 30.5 | 18.5 | 12.3 | 8.96 |


<hr>
\section 下采样拷贝时间
下采样拷贝虽然点数少，但是需要多次拷贝操作，那么这种方式和全拷贝速度差异如何呢？

\code{cpp}
uint32_t W=640, H=480, WH=W*H;
FILE *f = fopen("img","rb");
uint8_t* buf = malloc(WH);
fread(buf, WH, 1, f);

int64_t t = cv::getTickCount();
uint8_t * img = (uint8_t*) malloc(WH);
memcpy(img, buf, WH);
t = cv::getTickCount() - t;
cout<<t/cv::getTickFrenquency()<<endl;

t = cv::getTickCount();
uint32_t sd=5, Wd=W/sd, Hd=H/sd;
uint8_t *img_d = (uint8_t*)malloc(Wd*Hd);
for(uint32_t r=0; r<Hd; r++)
  for(uint32_t c=0; c<Wd; c++)
    img_d[r*Wd+c] = buf[r*sd*W+c*sd];
t = cv::getTickCount()-t;
cout<<t/cv::getTickFrequency()<<endl;
\endcode

PC平台结果：

| sd   | 1    | 2    | 3    | 4    | 5    | 6    |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| t1   | 74.4 | 72.7 | 74.3 | 74.6 | 77.3 | 75.1 |
| t2   | 158  | 62.6 | 30.5 | 18.5 | 12.3 | 8.96 |

ARM平台结果：
| sd   | 1    | 2    | 3    | 4    | 5    | 6    |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| t1   | 644  | 654  | 652  | 645  | 670  | 685  |
| t2   | 667  | 208  | 97.6 | 28.2 | 69.0 | 48.4 |

【结论】虽然降采样拷贝的时间要比全拷贝短，但是并没有短到和拷贝的像素数量的线性关系，例如PC平台的2次降采样和memcpy全采样运行时间几乎一样。
这说明memcpy肯定不是逐像素拷贝，有某种加速方法。