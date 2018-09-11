void FWT(int a[],int n)    
{    
    for(int d=1;d<n;d<<=1)    
        for(int m=d<<1,i=0;i<n;i+=m)    
            for(int j=0;j<d;j++)    
            {    
                int x=a[i+j],y=a[i+j+d];    
                a[i+j]=(x+y)%mod,a[i+j+d]=(x-y+mod)%mod;    
                //xor:a[i+j]=x+y,a[i+j+d]=(x-y+mod)%mod;    
                //and:a[i+j]=x+y;    
                //or:a[i+j+d]=x+y;    
            }    
}    
    
void UFWT(int a[],int n)    
{    
    for(int d=1;d<n;d<<=1)    
        for(int m=d<<1,i=0;i<n;i+=m)    
            for(int j=0;j<d;j++)    
            {    
                int x=a[i+j],y=a[i+j+d];    
                a[i+j]=1LL*(x+y)*rev%mod,a[i+j+d]=(1LL*(x-y)*rev%mod+mod)%mod;    
                //xor:a[i+j]=(x+y)/2,a[i+j+d]=(x-y)/2;    
                //and:a[i+j]=x-y;    
                //or:a[i+j+d]=y-x;    
            }    
}    
void solve(int a[],int b[],int n)    
{    
    FWT(a,n);    
    FWT(b,n);    
    for(int i=0;i<n;i++) a[i]=1LL*a[i]*b[i]%mod;    
    UFWT(a,n);    
}    



void FWT(int arr[]) {
    for (int b = 0; b < maxlogk; ++b) {
        int m = 1 << b;
        for (int i = 0; i < (1 << maxlogk); ++i) if ((i & m) == 0) {
            int x = arr[i], y = arr[i ^ m];
            arr[i] = ADDX_MOD(x, y);
            arr[i ^ m] = SUBX_MOD(x, y);
        }
    }
}
void IFWT(int arr[]) {
    for (int b = 0; b < maxlogk; ++b) {
        int m = 1 << b;
        for (int i = 0; i < (1 << maxlogk); ++i) if ((i & m) == 0) {
            int x = arr[i], y = arr[i ^ m];
            arr[i] = MUL_MOD(inv2, ADDX_MOD(x, y));
            arr[i ^ m] = MUL_MOD(inv2, SUBX_MOD(x, y));
        }
    }
}
