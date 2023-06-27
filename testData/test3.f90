module constants  
implicit none 

   real, parameter, private :: pi = 3.1415926536  
   real, parameter, private :: e = 2.7182818285 
   
contains      
   subroutine show_consts()          
      print*, "Pi = ", pi          
      print*, "e = ", e     
   end subroutine show_consts 
   
   function ePowerx(x)result(ePx) 
   implicit none
      real::x
      real::ePx
      ePx = e ** x
   end function ePowerx
    
   function areaCircle(r)result(a)  
   implicit none
      real::r
      real::a
      a = pi * r**2  
   end function areaCircle
    
end module constants 


program module_example     
use constants      
implicit none     

   call show_consts() 
   
   print*, "e raised to the power of 2.0 = ", ePowerx(2.0)
   print*, "Area of a circle with radius 7.0 = ", areaCircle(7.0)  
   
end program module_example
