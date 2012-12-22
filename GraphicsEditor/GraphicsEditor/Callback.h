#ifndef _CALLBACK_H
#define _CALLBACK_H

/*
* Finally, a class that can be used to define callback functions for C++ classes. I love this class...Very useful for the callback functions used by the Translate and Scale classes
		http://www.partow.net/programming/templatecallback/index.html#TheSolution
*/

template <class Class, typename ReturnType, typename Parameter1, typename Parameter2>
class SingularCallBack
{
public:

   typedef ReturnType (Class::*Method)(Parameter1, Parameter2);

   SingularCallBack(Class* class_instance, Method method)
   : class_instance_(class_instance),
     method_(method)
   {}

   ReturnType operator()(Parameter1 parameter1, Parameter2 parameter2)
   {
      return (class_instance_->*method_)(parameter1, parameter2);
   }

   ReturnType execute(Parameter1 parameter1, Parameter2 parameter2)
   {
      return operator()(parameter1, parameter2);
   }

private:

   Class* class_instance_;
   Method method_;
};

#endif