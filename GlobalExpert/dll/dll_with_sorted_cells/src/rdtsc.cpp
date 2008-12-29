__declspec(naked) __int64 rdtsc()
{
  __asm
   {
   rdtsc
   ret
   }
}
