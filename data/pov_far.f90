program gerapov
  real,allocatable::x(:),y(:),z(:),angle(:)
!  real ::xm,ym,t
  real :: comp
  integer :: nr,nc,n
  real:: vx,vy
  character::cor*20
  real::auxiliar
  character ::a*20,fmtcam*60,fmtesf*60,arquivopov*12
  character :: define*20,leitura_nao_utilizada*20,var*20,ultimo*4
  real::valor
  integer::novo_pov = 0,i
  real::t
  real::look_x, look_y, look_z
  real:: disc_x, disc_y
  open(10,file="posicoes.dat")
!  open(20,file="obs")
!  read(20,*)n,nv,nc,comp
!  close(20)
  open(20,file="../include/param.h")
  i=0
  do while(i==0)
    read(20,*,END=20,ERR=30)define,var,valor
    !write(*,*)define,var,valor
    goto 40
    30  read(20,*,END=20,ERR=20)define,var,leitura_nao_utilizada
    40  continue
    write(*,*)define,var,valor
    if(var=="Lx")ladox=valor
    if(var=="Ly")ladoy=valor
    if(var=="R_EQ")r=valor
    if(var=="R_ESFERA")look_z=valor
    if(var=="L_CENTRO_X")disc_x=valor
    if(var=="L_CENTRO_Y")disc_y=valor
    if(var=="RESEVOIR_DENSITY")densidade=valor
  end do
  20 continue

!  look_x = ladox / 2.0
  look_x = 4.0*disc_x
  look_y = disc_y
  
!  look_x = lado_x
  ladoz = 10.0*disc_x
  
!  ladox = 2.0*look_x
!  ladoz = ladox
!  if(ladox<ladoy)then
!    ladoz=ladox*1.8
!  else
!    ladoz=ladox*0.8
!  end if
  write(*,*)"dimensoes do sistema:",ladox,ladoy,ladoz
  call system("rm ray*.pov")
  call system("ls ray*.png > toto")
  open(40,file="toto")
  do while(i==0)
    read(40,*,ERR=50,END=50)var
    ultimo(1:4) = var(4:7)
    ! read(ultimo,*,iostat=stat)  novo_pov
     read(ultimo,*)  novo_pov
  end do
  50 continue
  write(*,*)"ultimo que existe:",novo_pov,var
  !ladox=40;ladoy=80;ladoz=50
!  ladox=comp;ladoy=comp/2.0;ladoz=ladox
  do lup=1,1000
     !do i=1,7
    read(10,*)t,n

    allocate(x(n+1),y(n+1),z(n+1),angle(n+1))
     !end do
     do i=1,n
       read(10,*)x(i),y(i),vx,vy,a
       angle(i)=atan2(vy,vx)
       if(angle(i)>6.2830)angle(i)=angle(i)-6.2830
       if(angle(i)<0)angle(i)=angle(i)+6.2830
     end do
     if(lup>novo_pov)then
       write(*,*)lup,n
       r=0.4
       z=r
       ! exporta .pov
       write(arquivopov,'(a3,i4.4,a4)')'ray',lup,'.pov'
       !write(*,*)arquivopov
       open(30, file = arquivopov)
       fmtcam = '(a16,f10.5,a1,f10.5,a1,f10.5,a9,f10.5,a1,f10.5,a1,f10.5,a2)'
       write(30,*) '#include "colors.inc"'
       write(30,*) '// tempo:',t
!       write(30,fmtcam)'camera{location<',ladox/2.,',',1.0*ladoz,',',ladoy/2.,'>look_at<',ladox/2.,',',r,',',ladoy/2.,'>}'
       write(30,fmtcam)'camera{location<',look_x,',',1.0*ladoz,',',look_y,'>look_at<',look_x,',',r,',',look_y,'>}'
       !write(30,fmtcam)'camera{location<',ladox/2.,',',ladoy/2.,',',2*ladoz,'>look_at<',ladox/2.,',',ladoy/2.,',',ladoz/2.,'>}'
       write(30,'(a14,f10.5,a1,f10.2,a1,f10.5,a13)')'light_source{<',1.0*look_y,',',2.0*ladoz,',',1.0*ladox,'>color White}'
       !write(30,'(a14,f10.5,a1,f10.5,a1,f10.5,a13)')'light_source{<',-2*ladox,',',0.,',',-4.*ladoz,'>color White}'
       write(30,'(a10,f10.5,a35)') 'plane { y,', 0., ' pigment{color rgb<0.0, 0.0, 0.0>}}'
       fmtesf = '(a8,f12.7,a1,f5.2,a1,f12.7,a2,f10.5,a23,a13,a30)'
!       fmtesf = '(a8,f12.7,a1,f5.2,a1,f12.7,a2,f10.5,a150)'
       !!!  r =  RAIO DE EQUILIBRIO  !!!
       write(30,*)"disc{<",disc_x,",",r,",",disc_y,">, <0.0, 1.0, 0.0>, ",look_z,", pigment{ color rgb<0.5, 0.5 ,0.5>}}"
       do i = 1, n
!       if(mod(i,10)==0)then
         auxiliar=360*(angle(i))/(6.2830)
         write(cor,'(a1,i3,a9)')'<',int(auxiliar),',1.0,0.5>'
!         write(30,fmtesf)'sphere{<',x(i),',',z(i),',',y(i),'>,',r,'pigment{color rgb<1.0, 1.0, 1.0>}finish{phong 1}}'
         write(30,fmtesf)'sphere{<',x(i),',',z(i),',',y(i),'>,',r,'pigment{color CHSL2RGB(',cor,')}finish{phong 1}}'
!       end if
       end do
!     do i=nr+1,nc+nr
       !write(30,fmtesf)'sphere{<',x(i),',',y(i),',',z(i),'>,',r,'pigment{color Cyan}finish{phong 1}}'
!       write(30,fmtesf)'sphere{<',x(i),',',z(i),',',y(i),'>,',r,'pigment{color Cyan}finish{phong 1}}'
!     end do
       close(30)
     end if
     deallocate(x,y,z,angle)
   end do
   call system("rm toto")
end program gerapov
