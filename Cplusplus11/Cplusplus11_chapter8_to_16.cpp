//	----------------------------------------------------
//	Chapter12
//	对象间的依赖关系:依赖 -> 关联 -> 聚合 -> 组合 -> 继承(耦合关系递增)
//	消息总线技术:对象间通过消息联系，而不是直接依赖或关联，从而大大降低了对象间的耦合性
//				通用的消息格式定义:消息主题 + 泛型函数类型对象
//								(消息分发)	(消息接收者)