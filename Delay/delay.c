void delay_us(unsigned int dlyus)       // Function to generate a delay in microseconds
{
for(dlyus*=12;dlyus>0;dlyus--);          // Multiplies the required delay by 12 and continuously decrements until the loop completes
}

void delay_ms(unsigned int dlyms)       // Function to generate a delay in milliseconds
{
for (dlyms*=12000;dlyms>0;dlyms--);     // Multiplies the required delay by 12000 and continuously decrements until the loop completes
}

void delay_s(unsigned int delays)        // Function to generate a delay in seconds
{
for(delays*=12000000;delays>0;delays--); // Multiplies the required delay by 12000000 and continuously decrements until the loop completes
}
