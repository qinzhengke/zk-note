Python画图库：matplotlib{#matplotlib}
=======================

\section 如何设置显示范围？
~~~{python}
axes = plt.gca()
axes.set_xlim([xmin,xmax])
axes.set_ylim([ymin,ymax])
~~~

\section 如何打开网格？
plt.grid()

\section 如何设置曲线置顶？
zorder=99，数字越大，越置顶。
