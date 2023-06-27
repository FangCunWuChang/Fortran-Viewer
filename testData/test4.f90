program arrayConstruction
implicit none
   interface
      subroutine write_array (a)
         real :: a(:,:)
      end subroutine write_array
      
      subroutine write_l_array (a)
         logical :: a(:,:)
      end subroutine write_l_array
   end interface

   real, dimension(2,3) :: tsource, fsource, result
   logical, dimension(2,3) :: mask
   
   tsource = reshape( (/ 35, 23, 18, 28, 26, 39 /), &
                    (/ 2, 3 /) )
   fsource = reshape( (/ -35, -23, -18, -28, -26, -39 /), &
                    (/ 2,3 /) )
   mask = reshape( (/ .true., .false., .false., .true., &
                 .false., .false. /), (/ 2,3 /) )

   result = merge(tsource, fsource, mask)
   call write_array(tsource)
   call write_array(fsource)
   call write_l_array(mask)
   call write_array(result)
   
end program arrayConstruction



subroutine write_array (a)

   real :: a(:,:)
   do i = lbound(a,1), ubound(a,1)
      write(*,*) (a(i, j), j = lbound(a,2), ubound(a,2) )
   end do
   return
   
end subroutine write_array


subroutine write_l_array (a)

   logical :: a(:,:)
   do i = lbound(a,1), ubound(a,1)
      write(*,*) (a(i, j), j= lbound(a,2), ubound(a,2))
   end do
   return
   
end subroutine write_l_array