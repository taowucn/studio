/tmp/lingd/ldd/scull # ./scull_load                                                                        
scullsingle registered at fd00008                                                                          
sculluid registered at fd00009                                                                             
scullwuid registered at fd0000a                                                                            
sullpriv registered at fd0000b                                                                             
/tmp/lingd/ldd/scull # lsmod                                                                               
Module                  Size  Used by                                                                      
scull                  15448  0                                                                            
/tmp/lingd/ldd/scull # cat /proc/devices                                                                   
Character devices:                                                                                         
  1 mem                                                                                                    
  2 pty                                                                                                    
  3 ttyp                                                                                                   
  4 /dev/vc/0                                                                                              
  4 tty                                                                                                    
  4 ttyS                                                                                                   
  5 /dev/tty                                                                                               
  5 /dev/console                                                                                           
  5 /dev/ptmx                                                                                              
  6 lp                                                                                                     
  7 vcs                                                                                                    
 10 misc                                                                                                   
 13 input                                                                                                  
 14 sound                                                                                                  
 29 fb                                                                                                     
 81 video4linux                                                                                            
 90 mtd                                                                                                    
 99 ppdev                                                                                                  
116 alsa                                                                                                   
128 ptm                                                                                                    
136 pts                                                                                                    
180 usb                                                                                                    
189 usb_device                                                                                             
204 s3c2410_serial                                                                                         
253 scull                                                                                                  
253 scullp                                                                                                 
253 sculla                                                                                                 
254 usb_endpoint                                                                                           
                                                                                                           
Block devices:                                                                                             
  1 ramdisk                                                                                                
  7 loop                                                                                                   
 31 mtdblock                                                                                               
179 mmc                                                                                                    
180 ub                                                                                                     
/tmp/lingd/ldd/scull # ./test                                                                              
write size: 12                                                                                             
read: hello,lingd!                                                                                         
/tmp/lingd/ldd/scull # ./scull_unload                                                                      
/tmp/lingd/ldd/scull # lsmod                                                                               
Module                  Size  Used by                                                                      
/tmp/lingd/ldd/scull # ./scull_unload scull_quantum=6                                                      
rmmod: scull: No such file or directory                                                                    
rmmod: scull_quantum=6: No such file or directory                                                          
/tmp/lingd/ldd/scull # ./scull_load scull_quantum=6                                                        
scullsingle registered at fd00008                                                                          
sculluid registered at fd00009                                                                             
scullwuid registered at fd0000a                                                                            
sullpriv registered at fd0000b                                                                             
/tmp/lingd/ldd/scull # ./test                                                                              
write size: 6                                                                                              
write size: 6                                                                                              
read: hello,                                                                                               
read: lingd!    


/tmp/lingd/ldd/scull # ./scull_load scull_quantum=5                                                        
scullsingle registered at fd00008                                                                          
sculluid registered at fd00009                                                                             
scullwuid registered at fd0000a                                                                            
sullpriv registered at fd0000b                                                                             
/tmp/lingd/ldd/scull # ./test                                                                              
write size: 5                                                                                              
write size: 5                                                                                              
write size: 2                                                                                              
read: hello                                                                                                
read: ,ling                                                                                                
read: ,l              
