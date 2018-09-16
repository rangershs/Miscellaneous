//	----------------------------------------------------
//	std::thread的detach方法使线程分离，不像join会阻塞当前线程的执行
//但std::thread对象离开作用域后会析构，必须保证在线程函数运行中该对象是有效的，不然程序会发生异常

//	当前线程休眠 std::this_thread::sleep_for(std::chrono::sleep(n))

//	递归锁std::recursive_mutex
//允许同一线程多次获取互斥锁，比如一个类的多个函数互相调用或同时调用就会多次上锁
//(不推荐)效率低，任意使用会导致业务逻辑变复杂，并且允许上锁的次数不定

//	condition_variable与condition_variable_any相比，前者比后者的效率高，后者可以与更多类型的lock配合使用，更加灵活
//	[ std::unique_lock<std::mutex> ]

//	notify_one	唤醒其中一个休眠的线程获取互斥锁的所有权
//	notify_all	唤醒所有休眠的线程，多个线程争抢互斥锁的所有权，在满足一定条件后执行相应的操作，若条件不满足则会继续休眠

//	多线程中只调用一次模块
template<typename Fn,typename... Args>
void call_once(std::once_flag& flag,Fn&& fn,Args... args)
//	拥有相同flag的多线程运行时，fn只会执行一次

//	获取线程函数的返回值 --> std::future,std::promise,std::packaged_task
//	std::promise绑定std::future和对象，std::packaged_task绑定std::future和函数对象，绑定后它们共享状态值 shared state
//	(std::thread and std::future can not be copied,but they can be moved,shared_future can be copied)

//	std::async()异步创建线程，返回std::future对象，通过std::future的方法可以获取异步的执行状态和执行结果