# 用于二轮编码器和红外传感器小车的地图坐标校正库

## 由编码计算位置
一个由编码器电机二轮驱动的小车，如果认为其轮子很少打滑，并且两侧电机和编码器拥有相同的物理特性，
即编码器计数加减一时，导致的小车旋转、前后的增益是相等的话，
其小车方向就可以由两个编码器的读数之差来表示，而其两个每次编码器加减一时，对方向有相反的效果，
而对当前方向前进有相同的效果。我们就可以用编码器来计算当前小车的近似位置和方向。

其伪代码如下
```
常数 圆周分度 = 小车一个轮子不动，旋转一圈时另一个轮子的读数

变量 左侧编码计数
变量 右侧编码计数

变量 横坐标
变量 纵坐标

函数 当前角度 = (右侧编码计数 - 左侧编码计数) * 2 * PI / 圆周分度

中断 当左侧编码器前进一时:
    横坐标 += cos(当前角度())
    纵坐标 += sin(当前角度())
    左侧编码计数 ++
    
中断 当左侧编码器后退一时:
    左侧编码计数 --
    横坐标 -= cos(当前角度())
    纵坐标 -= sin(当前角度())
    
中断 当右侧编码器前进一时:
    右侧编码计数 ++
    横坐标 += cos(当前角度())
    纵坐标 += sin(当前角度())
    
中断 当右侧编码器后退一时:
    横坐标 -= cos(当前角度())
    纵坐标 -= sin(当前角度())
    右侧编码计数 --
```
这里编码计数求和和位置求和之间的先后关系，是为了消除其正负一误差

适用于 MSP430 的示例代码参见 `encoder_sensor.c`

## 由红外传感器来提供地图交互信息
通常来说，如果一个二驱的编码器小车，仅凭借编码器，
通过积分运算就可以得到一个较为精确的方向和坐标值，
但是由于积分误差的存在，显然会随着行驶不断累积，直到严重偏离而令人无法接受

但是，只需要一块用于探测当前状态下周围是否存在地标的红外传感器，在合适的情况下，就可以完全消除累积误差。

当红外传感器检测到存在的地标时，我们就已经通过编码已经拥有了一个不那么可靠的位置和方向数据，
但是我们的数据可能告诉我们只是在地标附近而并没有到达它，显然是我们的数据出错了，
此时我们应该调整我们关于编码坐标空间和真实世界空间的坐标转换关系，以此消除我们在这段时间的行驶过程中造成的累积误差

问题是:  
1. 我们如何知道红外传感器检测到的地标在地图的哪儿，因为地标可能并不是一个点，而可能是多个点，乃至一条线，甚至是一个区域  
2. 在我们假定了传感器检测到的世界坐标位置之后，我们应该如何调整坐标映射，使得我们从编码那得到的传感器坐标空间的数据在变换后符合我们最近几次的红外结果  

对于问题1，一个简单而有效的方法是：查找当前位置的红外传感附近最近的地标点，出于性能考虑，我们可能需要手动对地图进行编码，
或者使用包括但不限于空间划分树、平面四叉树，层次包围盒树等数据结构对我们需要查找最邻近地标的地图进行划分，或者使用地标索引场来加速求临近的过程。  
求临近点的加速算法和计算机图形学中的求交加速具有非常相似的特性，已经具有了成熟的算法，这里不再赘述。  
关于2022年浙江省赛的小车地图，已经通过手动的空间划分来完成了加速最邻近算法，参见 `near_map.c`

对于问题2，一个可行的思路是，保留最近几次的校正点信息，综合计算出两个坐标系之间的变换关系，这也是这个库的核心内容，
我们的主要思路是计算最近几次的校正点，在传感器空间和世界空间中的重心之差，作为两者的坐标平移量，
而计算每个点在两个空间的旋转角度之差，按照距离各自重心的距离作为权重加权平均，作为旋转偏移量。

## 注意事项
校正点缓冲仅保存最近一些校准点的信息，由于其太大会影响性能，而在点在一个地图局部过于密集的情况下效果可能非常差，因此你应当将红外数据进行边沿检测，
仅当进入退出地标线时才进行校准，或者设置最邻近校准时间，避免在一小块地方连续校准而导致精度下个。
