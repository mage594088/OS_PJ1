# OS_PJ1

李建德(r08922180)

零、	目標Goal
學習撰寫程式運行時需要之系統輔助system calls、並能夠編譯Linux kernel，最終再比較理論值與實驗值差異、並分析其可能原因。

一、	設計
使用兩核心，分別執行排程與指令『空等』，並以不同測資，運行不同排程方式(FIFO, PSFJ, RR, SJF)：

#define __NR_gettime 333：新增的system call之編號
#define __NR_printtime 334：新增的system call之編號

void gettime：使用新增的system call取得系統時間
void printtime：使用新增的system call印出時間在dmesg
void time_msg：以要求之格式，輸出pid, 開始時間、結束時間等資訊
void Run*：對應不同排程方式，執行其排程(*表：FIFO, PSJF, RR, SJF)

二、	核心版本
使用HW1的環境：
Oracle VM VirtualBox: Version 6.1.4 Edition.
Ubuntu (64-bit): 16.04.4 LTS.
Kernel: Linux 4.14.25.

新增內容：
1.	arch/x86/entry/syscalls/syscall_64.tbl 新增system call名稱與編號(333 common gettime sys_gettime與334 common printtime sys_printtime)。
2.	include/linux/syscalls.h 新增asmlinkage 資料(asmlink long sys_gettime(struct timespec *ts); 與asmlinkage long sys_printtime(char *str, int pid, struct timespec *start, struct timespec *end); )。
3.	新增gettime, printtime資料夾，內放gettime與printtime的Makefile, .c, .h檔，執行時會自動生成 .o檔。
4.	重新編譯kernel並重開機後，即可使用make, make run, make run1~5, make allfile…等指令，來編譯main.c, run.c, run.h等檔案，最終完成目標。
 
三、	實驗結果
以下將以要求之五組測試資料作說明 (TIME_MEASURE.txt, FIFO_1.txt, PSJF_2.txt, RR_3.txt, SJF_4.txt)，若有不清楚，可直接參考excel檔案，裡頭有更詳細的內容、包含圖片、公式是如何計算的 ：

![image] (https://imgur.com/E6NHRmL)
![image] (https://imgur.com/LXlT60J)

1.	單個Process之執行時間
i.	計算理論值：
分配先將各排程方法與測資之Turnaround值計算出來：

![image] (https://imgur.com/SSGS14d) 
![image] (https://imgur.com/XvOX6AB)
![image] (https://imgur.com/h2VY5Fc)
![image] (https://imgur.com/undefined)
![image] (https://imgur.com/undefined)
![image] (https://imgur.com/undefined)
 

ii.	分析dmesg紀錄：
將輸出之dmesg紀錄存至excel並拆解，利用mid(位置, 起啟位, 截取位數)，分別抓出process的編號、開始與結束時間，相減後可得『實際執行時間』 (差值=結束時間-開始時間)。

iii.	比較理論與實際值：
將每次實驗的第一個Process作類似正規化，轉換成Turnaround Time，再對餘下Processes也進行相同運算(excel圖表內深藍色數字組)，最後再將利用公式：abs(實際轉換值-理論值)/理論值，即可求得誤差(excel圖表內深綠色數字組)，可發現實際值與理論值非常相近。

不過必須特別說明：以上狀況在進行實驗時，依然還是有數據不合的可能出現，主要是某幾個process可能會執行超乎預期的久，推測應是因為實驗是以虛擬機來執行，故可能會受到host同時執行其他程式的影響，但通常只要再重新運行幾次就可以得到吻合結果。

2.	整體Program之執行時間
i.	計算理論值：
利用TIME_MEASUREMENT.txt之資料，將單個Process執行時間加總後平均，即可求出理想中的執行時間。

ii.	繪出圖形：
再利用i. 之理論時間，疊上實際執行時間，可得到下圖：
(綠色表理論值、藍色表實際值，淺色為Processes仍在等待時間，深色表Processes開始執行)。

(圖片為excel完成，並利用 powerpoint 疊圖產生)

![image] (https://imgur.com/q3GBGz7)

iii.	分析：
可能原因不外乎排程時間的延遲、資源獲取延遲，且誤差會隨著時間增加而累積，進而產生愈來愈慢、延遲愈來愈嚴重的結果。

四、	實驗過程筆記
1.	VM中若要更改cores數量，需先完全關閉電源(非儲存)，才可於VM介面上操作、新增cores，且愈多cores，編譯kernel速度愈快。
2.	sudo dmesg –c/C = sudo dmesg clean/clear。
3.	“<”, “>” 可以分別表示將已打好資料輸入到程式，及將stdout內容輸出至txt(因為一開始並不知道有這樣的指令，故都用fscandf/fprintf來寫…)。
4.	雖然HW1有提到：kernel/Makefile要加上obj-y += …，但意外發現沒加好像也還是可以執行，不過其他部分則就必須加(且若是有額外資料夾，也要到最外層Makefile裡去通知可以使用)。
5.	./main可能無法直接執行，需要使用sudo ./main。

五、	參考資料
1.	Makefile：https://www.youtube.com/watch?v=E1_uuFWibuM
2.	Windows上Makefile(MinGW)：https://www.itread01.com/content/1546865588.html
3.	CPU Affinity：https://www.gnu.org/software/libc/manual/html_node/CPU-Affinity.html
4.	waitpid：https://blog.csdn.net/Roland_Sun/article/details/32084825
