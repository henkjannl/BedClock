

int linearize(float intensity) {
  /*
  The function takes a float in the domain from 0..255 and translates it 
  to an integer output value between 0..2048

  a*exp(b*x)=y    
  a*exp(b*1)=1                            (1)
  a*exp(b*255)=16*128-1                   (2)
  
  (1)    a=1/exp(b)                       (3)
  (1+3)  1/exp(b)*exp(b*255)=16*90-1    
         exp(b*255)=(16*90-1)*exp(b)    
         ln(exp(b*255))=ln((16*90-1)*exp(b))    
         b*255=ln(16*90-1)+b    
         b*254=ln(16*90-1)    
         b=ln(16*90-1)/254    
  */
  const float b=log(16*90-1)/254;
  const float a=1/exp(b);
  
  return (int) (a*exp(b*intensity));
}
