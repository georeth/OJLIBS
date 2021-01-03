// IO optimization is tricky, because IO performance is heavily depend on compiler and libc.
// 1) std::cin, std::cout are slow without sync_with_stdio(0).
//    even with sync_with_stdio(0), performance is unstable among compilers.
// 2) printf, scanf should be fast enough to AC any problem.
//    if not, it's problem setter's fault.
// 3) using getc() + putc() + custom format functions (Cg / Cp) can beat printf / scanf on many settings.
//    if we use buffer + fread() + fwrite() (Cbg / Cbp), it is faster than printf() + scanf() in all known settings.
//    however, standard IO functions cannot be used if we use buffer,
//    so our fast io interface must general enough to replace stdio completely.
// 4) some benchmarks (small is better):
//    G++11 5.1.0: Cg = Cbg < scanf
//                 Cbp < Cp < printf
//    G++17 7.3.0: Cbg < scanf < Cg
//                 Cbp < printf < Cp
//    it seems putc() and getc() become quite slow in G++17 7.3.0, so we have to use buffer.
//    otherwise, Cg and Cp will be negaive optimization.
// 5) we use comma operator. it has lowest precedence, so no parenthesis is needed.
//    but using side effect for comma operator is dangerous anyway.

// assume already using namespace std;
// should compile for c++11
#include <cmath>
#include <iostream>
#include <vector>
#include <utility>
namespace fio {
static const char eof = '\xff';
#define LL long long
#define I inline
#define OP operator
#define RT return*this;
#define ALL_INT(mcr) mcr(int)mcr(long)mcr(LL)mcr(unsigned)mcr(unsigned long)mcr(unsigned LL)
#define ALL_FLT(mcr) mcr(float)mcr(double)mcr(long double)
#define S2D x=0;int c=P();while((c<'0'||c>'9')&&c!='-')c=P();bool h=false;if(c=='-')h=true,c=P()
#define R2D for(;c>='0'&&c<='9';c=P())x=x*10+c-'0'
#define R2F if(c=='.'){double u=0.1;for(c=P();c>='0'&&c<='9';c=P(),u*=0.1)x+=u*(c-'0');}
#define RD_INT(Int) I Fr&OP,(Int&x){S2D;R2D;if(h)x=-x;RT}
#define RD_FLT(Flt) I Fr&OP,(Flt&x){S2D;R2D;R2F;if(h)x=-x;RT}
struct Cg{I char OP()(){char c=(char)getchar();if(c == eof)valid=false;return c;};bool valid=true;};
struct Cbg{
    I char OP()(){
        if (b == e) {
            if (!valid) return eof;

            size_t sz = fread(buf, 1, BUFSIZE, stdin);
            if (!sz) valid = false;
            else {
                b = buf;
                e = buf + sz;
            }
        }
        if (b < e) return *b++;
        return eof;
    };
    bool valid=true;
    static const int BUFSIZE = 1024;
    char buf[BUFSIZE];
    char *b = buf, *e = buf;
};
struct Cbp{
    I void OP()(int x){
        if (e == buf_end) {
            fwrite(buf, 1, e - buf, stdout);
            e = buf;
        }
        *e++ = (char)x;
    }
    ~Cbp() { fwrite(buf, 1, e - buf, stdout); e = buf; }

    static const int BUFSIZE = 1024;
    char buf[BUFSIZE];
    char *e = buf;
    const char *buf_end = buf + BUFSIZE;
};
struct Cis{
    I char OP()(){
        if (i < s.size()) return s[i++];
        valid = false;
        return eof;
    };
    bool valid = true;
    string s;
    int i = 0;
};
struct Cos{
    I void OP()(int x){
        s.push_back((char)x);
    };
    string s;
};
struct Cp{I void OP()(int x){putchar(x);}};
struct Ce{I void OP()(int x){putc(x,stderr);}};
I bool IS(char x){return x==' '||x=='\r'||x=='\n'||x=='\t';}
template<class T>struct Fr{T P;ALL_INT(RD_INT)ALL_FLT(RD_FLT)I Fr&OP,(char&x){for(x=P();IS(x);)x=P();RT}
I Fr&OP,(char*s){for(*s=P();IS(*s)&&P.valid;)*s=P();for(;!IS(*s)&&P.valid;)*++s=P();*s=0;RT}
I Fr&OP,(string &s){s.clear();char c=P();for(;IS(c)&&P.valid;)c=P();for(;!IS(c)&&P.valid;)s.push_back(c),c=P();RT}
I OP bool() { return P.valid; }
string line() {string s; for (char c=P();c!=eof&&c!='\n';c=P())s.push_back(c);return s;}
template<class It>I Fr&OP()(It b, It e) {while (b < e) (*this),*b++; RT}
template<class T1>I Fr&OP,(vector<T1> &v) {(*this)(v.begin(), v.end()); RT}
template<class T1,class T2>I Fr&OP,(pair<T1,T2> &p) {(*this),p.first,p.second; RT}
#ifdef __cpp_fold_expressions
template<class... Ts>I Fr&OP,(tuple<Ts...> &t) { std::apply([this](auto &...ts){((void)((*this),ts),...);}, t); RT}
#endif
};
#define W4D if(x){if(x<0)P('-'),x=-x;int p=0;while(x)B[p++]=x%10+'0',x/=10;while(p--)P(B[p]);}else P('0')
#define WR_INT(Int) I Fw&OP,(Int x){W4D;RT}
#define WR_FLT(Flt) I Fw&OP()(Flt g,int d){if(d){double t=0.5;for(int i=d;i--;)t*=0.1;if(g<0)P('-'),g=-g;g+=t;*this,(LL)(g);P('.');g-=floor(g);while(d--){g*=10;int i=(int)floor(g);P(i+'0');g-=i;}}else*this,(g>0?(LL)(g+0.5):(LL)(g-0.5));RT} I Fw&OP,(Flt g){ return (*this)(g, prec); }
template<class T>struct Fw{T P;char B[32];ALL_INT(WR_INT)ALL_FLT(WR_FLT)I Fw&OP,(char x){P(x);RT}
I Fw&OP,(const char*s){for(;*s;++s)P(*s);RT}
I Fw&OP,(const string&s){(*this),s.c_str();RT}
template<class C>I Fw&all(const C &v, char sep=' ', char nl='\n'){auto it=v.begin();for(int i=0;i<(int)v.size();++i,++it){if(i&&sep)P(sep);(*this),*it;}if(nl)P(nl);RT}
template<class U>I Fw&print(U x){(*this),x;RT}
template<class U,class...Us>I Fw&print(U x,Us...args){(*this),x;P(' ');print(args...);RT}
template<class...Us>I Fw&println(Us...args){print(args...);P('\n');RT}
I Fw&set_prec(int p) { prec = p; RT }
int prec = 6;
};
#undef LL
#undef I
#undef OP
#undef RT
#undef ALL_INT
#undef ALL_FLT
#undef S2D
#undef R2D
#undef R2F
#undef RD_INT
#undef RD_FLT
#undef W4D
#undef WR_INT
#undef WR_FLT
}
// fio::Fr<fio::Cg>fin;
fio::Fr<fio::Cbg>fin;
fio::Fw<fio::Cbp>fout;
// fio::Fw<fio::Cp>fout;
fio::Fw<fio::Ce>ferr;

struct iss : fio::Fr<fio::Cis> {
    iss(const string &s) { P.s = s; }
    iss() = default;
};
struct oss : fio::Fw<fio::Cos> {
    string get() { return P.s; }
};
