program gravitationalDisp

    ! this program calculates vertical motion under gravity
    implicit none

    ! gravitational acceleration
    real, parameter :: g = 9.81

    ! variable declaration
    real :: s ! displacement
    real :: t ! time
    real :: u ! initial speed

    ! assigning values
    t = 5.0
    u = 50
            
    ! displacement
    s = u * t - g * (t ** 2) / 2

    ! output
    print *, "Time = ", t
    print *, 'Displacement = ', s

end program gravitationalDisp
