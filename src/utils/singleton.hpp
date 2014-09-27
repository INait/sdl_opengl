#ifndef _SINGLETON_HPP_
#define _SINGLETON_HPP_

template < typename T >
class ISingletonable
{
public:
	static T & GetInstance();
	static void SetInstance(T & instance);

private:
	static T * instance_ptr_;
};

#define DECLARE_SINGLETON_VAR( ClassName ) \
	template<> ClassName *ISingletonable< ClassName >::instance_ptr_ = 0;	\
	template<> ClassName & ISingletonable< ClassName >::GetInstance() { return *instance_ptr_; }	\
	template<> void ISingletonable< ClassName >::SetInstance( ClassName & instance ) { instance_ptr_ = & instance; }

#endif // _SINGLETON_HPP_