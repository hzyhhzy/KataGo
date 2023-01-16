#include "../tests/tests.h"

using namespace std;

bool TestCommon::boardsSeemEqual(const Board& b1, const Board& b2) {
  for(int i = 0; i < Board::MAX_ARR_SIZE; i++)
    if(b1.colors[i] != b2.colors[i])
      return false;
  return true;
}

string TestCommon::getBenchmarkSGFData(int boardSize) {
  string sgfData;
  static_assert(MIN_BENCHMARK_SGF_DATA_SIZE == 7, "TestCommon::getBenchmarkSGFData MIN_BENCHMARK_SGF_DATA_SIZE == 7");
  static_assert(MAX_BENCHMARK_SGF_DATA_SIZE == 19, "TestCommon::getBenchmarkSGFData MAX_BENCHMARK_SGF_DATA_SIZE == 19");

  if(boardSize == 19) {
    sgfData =
      "(;FF[4]GM[1]SZ[19]HA[0]KM[7.5];B[dd];W[pp];B[dp];W[pd];B[qq];W[pq];B[qp];W[qo];B[ro];W[rn];B[qn];W[po];B[rm];W["
      "rp];B[sn];W[rq];B[nc];W[oc];B[qr];W[rr];B[nd];W[pf];B[re];W[lc];B[jc];W[le];B[qc];W[qd];B[ob];W[pc];B[pb];W[rc];"
      "B[qb];W[rd];B[lb];W[mb];B[ld];W[kb];B[kc];W[la];B[mc];W[qi];B[ke];W[cc];B[dc];W[cd];B[cf];W[ce];B[de];W[bf];B["
      "cb];W[bb];B[be];W[db];B[bc];W[ca];B[bd];W[cb];B[bg];W[df];B[cg];W[fc];B[nr];W[nq];B[pg];W[qg];B[of];W[ef];B[mq];"
      "W[cq];B[dq];W[cp];B[co];W[bo];B[bn];W[cn];B[do];W[bm];B[bp];W[an];B[bq];W[cr];B[br];W[kq];B[iq];W[ko];B[kr];W["
      "cj];B[lq];W[bi];B[qj];W[ph];B[og];W[rf];B[gc];W[gd];B[hc];W[nb];B[rb];W[sb];B[lb];W[fe];B[ri];W[rh];B[pi];W[qh];"
      "B[af];W[lc];B[dn];W[dm];B[em];W[in];B[lb];W[fn];B[lc];W[en];B[fp];W[fm];B[pj];W[hp];B[hq];W[ij];B[di];W[dj];B["
      "jl];W[il];B[jm];W[im];B[jk];W[jj];B[kj];W[ki];B[lj];W[li];B[mi];W[jh];B[pn];W[or];B[np];W[ie];B[dh];W[ei];B[eh];"
      "W[fh];B[kn];W[oo];B[mn];W[oh];B[ni];W[oq];B[fg];W[gh];B[hg];W[hh];B[ig];W[ih];B[fb];W[eb];B[gb];W[mj];B[mg];W["
      "kf];B[lf];W[je];B[kg];W[kd];B[jg];W[ln];B[lm];W[lo];B[nn];W[ra];B[na];W[gp];B[gq];W[fo];B[rj];W[oe];B[me];W[mr];"
      "B[lr];W[ns];B[sp];W[on];B[om];W[pm];B[nm];W[ip];B[jp];W[jo];B[kp];W[lp];B[jq];W[ap];B[dr];W[ar];B[cs];W[aq];B["
      "sr];W[qs];B[ab];W[ea];B[ec];W[fd];B[fa];W[ee];B[ke];W[oi];B[oj];W[le];B[ik];W[hk];B[ke];W[ep];B[fq];W[le];B[eo];"
      "W[ke];B[sq];W[pr];B[qm];W[ml];B[mk];W[lk];B[nj];W[kk];B[mj];W[km];B[kl];W[qa];B[hd];W[he];B[gf];W[ge];B[bh];W["
      "nf];B[ng];W[ne];B[ci];W[ai];B[mf];W[eg];B[gg];W[dg];B[ch];W[si];B[ah];W[bj];B[sj];W[sh];B[ma];W[sc];B[pa];W[id];"
      "B[ic];W[ls];B[ks];W[ms];B[sa];W[ra];B[od];W[pe];B[kh];W[lh];B[ji];W[ii];B[mh];W[ji];B[lg];W[mp];B[no];W[jn];B["
      "km];W[as];B[fi];W[ej])";
  } else if(boardSize == 18) {
    sgfData =
      "(;FF[4]GM[1]SZ[18]HA[0]KM[7.5];B[od];W[do];B[oo];W[dd];B[cp];W[dp];B[co];W[cn];B[bn];W[bm];B[cm];W[dn];B[bl];W["
      "bo];B[am];W[bp];B[cq];W[bq];B[fc];W[pp];B[op];W[po];B[pm];W[pn];B[on];W[qm];B[pq];W[pc];B[qn];W[rn];B[ql];W[qo];"
      "B[pl];W[qq];B[pd];W[oc];B[nd];W[mb];B[cf];W[fd];B[gd];W[fe];B[dc];W[cc];B[ec];W[df];B[ed];W[de];B[cb];W[bd];B["
      "bc];W[cd];B[ch];W[bb];B[ba];W[ac];B[lc];W[lb];B[kc];W[mk];B[lm];W[mi];B[lj];W[mj];B[kl];W[mm];B[mn];W[nm];B[pi];"
      "W[ln];B[kn];W[lo];B[om];W[ko];B[jo];W[jp];B[io];W[ip];B[ho];W[hp];B[kh];W[oi];B[ph];W[oh];B[go];W[gp];B[fp];W["
      "fq];B[fo];W[lq];B[eq];W[dq];B[mp];W[pg];B[og];W[ng];B[of];W[pj];B[qj];W[qi];B[qh];W[qk];B[ri];W[pk];B[rk];W[ki];"
      "B[li];W[lh];B[kj];W[ji];B[jj];W[kg];B[jh];W[ii];B[jg];W[kf];B[gq];W[hq];B[hh];W[mg];B[fr];W[lp];B[eh];W[dm];B["
      "nk];W[oj];B[rl];W[dk];B[hf];W[gg];B[gh];W[fg];B[fh];W[db];B[da];W[ca];B[nq];W[ea];B[rp];W[rq];B[rm];W[qr];B[qn];"
      "W[cj];B[bj];W[qm];B[qc];W[qb];B[qn];W[pr];B[oq];W[qm];B[qd];W[gl];B[fm];W[gj];B[el];W[dl];B[ek];W[ej];B[fj];W["
      "ij];B[il];W[bi];B[bk];W[ci];B[cl];W[ih];B[ig];W[fk];B[fi];W[ik];B[jk];W[im];B[jl];W[jn];B[hm];W[km];B[ll];W[ml];"
      "B[in];W[qn];B[lf];W[le];B[mf];W[lg];B[kb];W[hg];B[je];W[ie];B[he];W[id];B[if];W[gc];B[hd];W[gb];B[ol];W[jd];B["
      "hc];W[kd];B[hb];W[md];B[mc];W[nb];B[me];W[jf];B[rb];W[oa];B[fb];W[cg];B[bh];W[bg];B[dh];W[ah];B[mo];W[ld];B[nf];"
      "W[jb];B[jc];W[ka];B[ic];W[ke];B[fl];W[eb];B[ga];W[hr];B[ia];W[ja];B[qa];W[pb];B[gk];W[kn];B[aj];W[ai];B[nj];W["
      "ni];B[jm];W[ao];B[an];W[nn];B[no];W[lk];B[dr];W[cr];B[en];W[er];B[ib];W[la];B[dr];W[mr];B[jq];W[jr];B[kr];W[kq];"
      "B[ir];W[iq];B[mq];W[jr];B[er];W[pa];B[rc];W[ra])";
  } else if(boardSize == 17) {
    sgfData =
      "(;FF[4]GM[1]SZ[17]HA[0]KM[7.5];B[dd];W[nn];B[nd];W[dn];B[oo];W[on];B[no];W[mo];B[mp];W[lp];B[lo];W[mn];B[kp];W["
      "np];B[lq];W[op];B[po];W[pp];B[cn];W[co];B[cm];W[do];B[dm];W[cc];B[cd];W[dc];B[fc];W[ec];B[ed];W[fb];B[bc];W[bb];"
      "B[gb];W[fd];B[gc];W[bd];B[pl];W[pm];B[be];W[ac];B[fe];W[nf];B[of];W[og];B[oe];W[ng];B[ld];W[kg];B[ig];W[ke];B["
      "go];W[fn];B[fl];W[fo];B[ji];W[bf];B[cf];W[ae];B[li];W[mj];B[lf];W[ne];B[md];W[od];B[pd];W[oc];B[pc];W[ob];B[pg];"
      "W[ph];B[pb];W[lg];B[mf];W[mg];B[kf];W[mb];B[lb];W[kb];B[lc];W[pf];B[pe];W[qg];B[ch];W[la];B[pa];W[jf];B[jg];W["
      "hb];B[jc];W[hc];B[gd];W[ga];B[jb];W[ce];B[hd];W[cg];B[bh];W[ho];B[ip];W[kk];B[mi];W[nj];B[ik];W[hl];B[hk];W[dg];"
      "B[dh];W[fg];B[eh];W[ff];B[gp];W[gn];B[hp];W[jo];B[jp];W[bo];B[bn];W[hf];B[if];W[ln];B[io];W[ko];B[hn];W[fp];B["
      "gh];W[ha];B[ib];W[jj];B[ij];W[jm];B[lp];W[il];B[gl];W[jk];B[kh];W[hg];B[hh];W[an];B[am];W[em];B[el];W[ao];B[fq];"
      "W[ep];B[ni];W[oi];B[eb];W[fa];B[ic];W[db];B[df];W[de];B[ef];W[eg];B[ee];W[bg];B[nq];W[in];B[pq];W[qo];B[oq];W["
      "pn];B[gq];W[lj];B[ki];W[he];B[ie];W[be];B[ge];W[ah];B[ai];W[ag];B[bj];W[qe];B[qd];W[hm];B[eq];W[dq];B[fh];W[me];"
      "B[le];W[qf];B[fm];W[en];B[ia];W[ea];B[nh];W[oh];B[ho];W[qq];B[mq];W[gm];B[qp];W[dl];B[cl];W[qq];B[kj];W[mh];B["
      "qp];W[dk];B[ck];W[qq];B[gk];W[gf];B[qp];W[qb];B[nb];W[qq];B[oj];W[pj];B[qp];W[nc];B[na];W[qq];B[lh];W[gg];B[qp];"
      "W[ma];B[oa];W[qq];B[dp];W[cp];B[qp];W[id];B[jd];W[qq];B[cq];W[bq];B[qp];W[bi];B[ci];W[qq];B[qi];W[ok];B[qp];W["
      "bm];B[bl];W[qq];B[qh];W[pg];B[qp];W[iq];B[jq];W[qq];B[pi];W[qj];B[qp];W[qa];B[mc];W[qq];B[da];W[ca];B[qp];W[jh];"
      "B[ii];W[qq];B[kn];W[jn];B[qp];W[aj];B[qq];W[bi];B[po];W[ai];B[qn];W[qm])";
  } else if(boardSize == 16) {
    sgfData =
      "(;FF[4]GM[1]SZ[16]HA[0]KM[7.5];B[mm];W[dd];B[md];W[dm];B[cc];W[cd];B[dc];W[ed];B[fb];W[ck];B[nf];W[nn];B[mn];W["
      "nm];B[nl];W[ol];B[ok];W[nk];B[ml];W[oj];B[om];W[pk];B[on];W[no];B[oo];W[fn];B[bc];W[gd];B[ni];W[mk];B[dj];W[cj];"
      "B[dh];W[gh];B[ci];W[cg];B[dk];W[bi];B[dl];W[cl];B[em];W[en];B[hi];W[gi];B[hk];W[hj];B[fm];W[gm];B[fk];W[ij];B["
      "dn];W[cm];B[gn];W[go];B[hn];W[ho];B[in];W[io];B[jn];W[jo];B[do];W[eo];B[ep];W[fo];B[cn];W[kn];B[bm];W[jm];B[gj];"
      "W[gl];B[bl];W[bk];B[gk];W[al];B[bn];W[mc];B[nc];W[lc];B[nb];W[me];B[nd];W[ld];B[hc];W[ch];B[di];W[fi];B[jj];W["
      "ii];B[jl];W[mo];B[lo];W[ll];B[ln];W[im];B[hl];W[hm];B[kk];W[lk];B[kh];W[fl];B[ig];W[ih];B[jh];W[ie];B[el];W[lg];"
      "B[mh];W[jg];B[kg];W[jf];B[kf];W[ke];B[lf];W[le];B[og];W[bd];B[jd];W[je];B[hd];W[li];B[lj];W[mj];B[kj];W[ac];B["
      "ab];W[ad];B[cb];W[he];B[dg];W[jb];B[am];W[mi];B[bj];W[aj];B[ak];W[lm];B[np];W[al];B[cf];W[bf];B[ak];W[ko];B[mp];"
      "W[al];B[kl];W[oi];B[ak];W[gb];B[gc];W[al];B[nh];W[km];B[ak];W[ne];B[oe];W[al];B[hg];W[gg];B[ak];W[fc];B[ec];W["
      "fd];B[hb];W[al];B[hh];W[hf];B[ak];W[eb];B[ga];W[al];B[ik];W[if];B[ak];W[ca];B[ea];W[al];B[mb];W[ak];B[lb];W[lh];"
      "B[mf];W[id];B[kb];W[ao];B[bo];W[cp];B[dp];W[bp];B[jc];W[ic];B[ib];W[kc];B[ef];W[ja];B[bg];W[bh];B[be];W[af];B["
      "ce];W[ff];B[ee];W[fe];B[fj];W[ei];B[ej];W[oh];B[ph];W[pi];B[pg];W[kd];B[pm];W[mg];B[ng];W[ae];B[pl];W[ok];B[fp];"
      "W[eg];B[df];W[eh];B[gp];W[ia];B[ha])";
  } else if(boardSize == 15) {
    sgfData =
      "(;FF[4]GM[1]SZ[15]HA[0]KM[7.5];B[dd];W[ll];B[ld];W[dl];B[dm];W[cm];B[em];W[cl];B[im];W[mc];B[lc];W[md];B[le];W["
      "nf];B[el];W[km];B[dj];W[bj];B[mj];W[mg];B[nl];W[mm];B[kk];W[jl];B[nm];W[il];B[hm];W[hl];B[bi];W[cj];B[ci];W[gm];"
      "B[gn];W[gl];B[ei];W[fn];B[cn];W[bn];B[en];W[co];B[fm];W[dc];B[ec];W[cd];B[ed];W[cc];B[in];W[ii];B[cf];W[ce];B["
      "df];W[eb];B[fb];W[da];B[ki];W[ig];B[lg];W[gh];B[gj];W[jj];B[kj];W[fj];B[fk];W[gf];B[gi];W[hd];B[hh];W[ih];B[hi];"
      "W[hj];B[hg];W[hf];B[if];W[gk];B[fi];W[lh];B[ie];W[kg];B[lf];W[kh];B[gc];W[nn];B[nh];W[mh];B[ni];W[ml];B[mk];W["
      "om];B[ok];W[on];B[ng];W[og];B[lk];W[li];B[kl];W[mf];B[mn];W[lm];B[no];W[ln];B[mo];W[lo];B[jm];W[oo];B[mo];W[jn];"
      "B[jo];W[ko];B[kn];W[id];B[mn];W[jc];B[kb];W[jb];B[mb];W[nb];B[je];W[gd];B[na];W[nc];B[ff];W[jd];B[hc];W[ke];B["
      "kf];W[kd];B[gg];W[he];B[jf];W[fe];B[ja];W[ia];B[la];W[ka];B[dn];W[ef];B[de];W[bl];B[ja];W[ib];B[fg];W[fd];B[fc];"
      "W[ka];B[jk];W[ik];B[ja];W[kc];B[hb];W[me];B[ee];W[jg];B[lb];W[ka];B[bo];W[ao];B[ja];W[oa];B[ob];W[ka];B[am];W["
      "dk];B[ja];W[bf];B[ha];W[ka];B[an];W[bo];B[ja];W[ek];B[ej];W[ka];B[aj];W[ak];B[ja];W[ai];B[bg];W[be];B[af];W[ka];"
      "B[ji];W[ij];B[ja];W[ae];B[ag];W[ka];B[ea];W[ma];B[bb];W[cb];B[db];W[ca];B[bc];W[eb];B[fa];W[ab];B[ac])";
  } else if(boardSize == 14) {
    sgfData =
      "(;FF[4]GM[1]SZ[14]HA[0]KM[7.5];B[dk];W[kd];B[kk];W[dd];B[dc];W[cc];B[ec];W[cb];B[ic];W[ed];B[fd];W[jc];B[gc];W["
      "ef];B[id];W[le];B[cg];W[bf];B[il];W[cl];B[dl];W[ck];B[cj];W[bj];B[bi];W[ci];B[dj];W[bh];B[bk];W[ai];B[bl];W[cm];"
      "B[bm];W[di];B[lg];W[jb];B[lj];W[fe];B[ei];W[eh];B[fi];W[kg];B[kh];W[he];B[ib];W[jg];B[hd];W[jh];B[ja];W[lh];B["
      "kb];W[lc];B[lb];W[mb];B[db];W[hk];B[hi];W[ik];B[jk];W[hl];B[ij];W[jl];B[jm];W[hj];B[ii];W[gi];B[gh];W[gj];B[fh];"
      "W[im];B[km];W[em];B[dm];W[fk];B[gm];W[hh];B[fj];W[el];B[gl];W[en];B[gk];W[hm];B[gn];W[cn];B[dn];W[ej];B[bn];W["
      "ek];B[gf];W[jj];B[eg];W[ih];B[dh];W[mi];B[in];W[ji];B[hn];W[ll];B[kl];W[mj];B[mk];W[ml];B[lk];W[nk];B[mm];W[nm];"
      "B[lm];W[bg];B[cd];W[ce];B[ge];W[bd];B[ma];W[mc];B[ca];W[ba];B[da];W[mn];B[li];W[mh];B[ki];W[mg];B[bb];W[bc];B["
      "aa];W[je];B[ff];W[ee];B[nl];W[ml];B[if];W[ie];B[hf];W[jf])";
  } else if(boardSize == 13) {
    sgfData =
      "(;FF[4]GM[1]SZ[13]HA[0]KM[7.5];B[dd];W[jj];B[kk];W[jd];B[kj];W[dj];B[jc];W[dc];B[cc];W[ec];B[ed];W[fc];B[fd];W["
      "ic];B[hc];W[kc];B[gc];W[ch];B[jb];W[ib];B[id];W[ie];B[hd];W[kb];B[je];W[ja];B[kd];W[jc];B[jf];W[jk];B[ji];W[ii];"
      "B[fk];W[kl];B[ll];W[il];B[km];W[dk];B[hk];W[jg];B[kg];W[jh];B[ki];W[gj];B[hj];W[gk];B[hl];W[kh];B[if];W[lg];B["
      "gi];W[li];B[jl];W[le];B[bg];W[bh];B[cg];W[dg];B[df];W[gb];B[dh];W[di];B[eg];W[cb];B[bb];W[db];B[bj];W[bi];B[aj];"
      "W[ai];B[cl];W[bk];B[dl];W[fj];B[fl];W[fi];B[cj];W[ck];B[bl];W[gh];B[hi];W[eh];B[ig];W[al];B[ek];W[ej];B[fg];W["
      "fh];B[ih];W[kf];B[ba];W[gl];B[gm];W[hb];B[ke];W[ld];B[gg];W[ag];B[bf];W[bc];B[cd];W[ca];B[bd];W[lj];B[lk];W[hh];"
      "B[hg];W[mk];B[ml];W[mj];B[dg];W[bm];B[cm];W[ci];B[am];W[ac];B[ad];W[bm];B[af];W[ah];B[am];W[em];B[ak];W[al];B["
      "fm];W[bm];B[ak];W[aj];B[am];W[ab];B[aa];W[bm];B[fb];W[fa];B[am];W[el];B[dm];W[bm];B[hm];W[ak];B[am];W[em];B[el];"
      "W[bm];B[ea];W[eb];B[am];W[gd])";
  } else if(boardSize == 12) {
    sgfData =
      "(;FF[4]GM[1]SZ[12]HA[0]KM[7.5];B[ii];W[dd];B[cc];W[cd];B[dc];W[ed];B[ec];W[di];B[fd];W[jd];B[cg];W[fe];B[gd];W["
      "bc];B[jf];W[hc];B[hd];W[ic];B[eg];W[be];B[fi];W[gf];B[gb];W[fj];B[gj];W[ej];B[gk];W[fh];B[fg];W[if];B[je];W[gi];"
      "B[gg];W[hi];B[hh];W[hj];B[ij];W[hk];B[ik];W[il];B[jl];W[hl];B[hb];W[ie];B[id];W[jg];B[kg];W[kh];B[ig];W[jh];B["
      "hf];W[ih];B[he];W[kf];B[ke];W[lg];B[bb];W[ab];B[ba];W[bg];B[bh];W[cf];B[ag];W[bf];B[ch];W[df];B[eh];W[ei];B[gh];"
      "W[jk];B[ck];W[le];B[ld];W[lf];B[kd];W[cj];B[bj];W[dk];B[ci];W[cl];B[ee];W[ef];B[ff];W[de];B[fi];W[kl];B[dj];W["
      "bk];B[cj];W[fk];B[ak];W[bl];B[af];W[ae];B[ah];W[fh];B[ge];W[fi];B[ee];W[cb];B[db];W[fe];B[ac];W[ad];B[ee];W[dl];"
      "B[aj];W[fe];B[li];W[ki];B[ee];W[ac];B[al];W[fe];B[ek];W[el];B[ee];W[aa];B[ca];W[fe];B[lh];W[lj];B[ee];W[ea];B["
      "da])";
  } else if(boardSize == 11) {
    sgfData =
      "(;FF[4]GM[1]SZ[11]HA[0]KM[7.5];B[hd];W[dh];B[dc];W[hh];B[cf];W[id];B[ie];W[he];B[hc];W[dd];B[cd];W[ec];B[de];W["
      "ed];B[db];W[ic];B[ib];W[jb];B[hb];W[je];B[if];W[jf];B[ig];W[ge];B[ih];W[fb];B[ja];W[kb];B[jd];W[jc];B[kd];W[ke];"
      "B[jg];W[kc];B[eb];W[fc];B[ee];W[fe];B[gg];W[ff];B[fi];W[ej];B[bh];W[hi];B[fg];W[fj];B[ef];W[gd];B[ii];W[cj];B["
      "ci];W[bj];B[gj];W[gi];B[hj];W[di];B[hg];W[ch];B[bg];W[bi];B[eh];W[dg];B[ea];W[gb];B[ai];W[aj];B[ah];W[kg];B[kh];"
      "W[ei];B[fh];W[gk];B[hk];W[fk];B[cg];W[eg];B[kf];W[cc];B[bc];W[df];B[ce];W[kg];B[jd];W[kd];B[cb];W[kf];B[hf];W["
      "ga];B[fa];W[ha];B[ia];W[gc];B[gf])";
  } else if(boardSize == 10) {
    sgfData =
      "(;FF[4]GM[1]SZ[10]HA[0]KM[7.5];B[gc];W[gg];B[dh];W[cd];B[dc];W[cg];B[ch];W[cc];B[he];W[hf];B[ie];W[fh];B[bg];W["
      "gd];B[hc];W[ec];B[db];W[dd];B[eb];W[bf];B[cf];W[dg];B[be];W[bh];B[af];W[bi];B[eg];W[df];B[ef];W[de];B[eh];W[ce];"
      "B[ci];W[bf];B[gf];W[hh];B[cf];W[bj];B[cj];W[bf];B[fi];W[gi];B[fg];W[gh];B[if];W[ei];B[cf];W[fe];B[ff];W[bf];B["
      "ah];W[ag];B[ii];W[ih];B[bg];W[hd];B[id];W[ag];B[ej];W[fj];B[bg];W[dj];B[cf];W[fc];B[fb];W[bf];B[di];W[ag];B[ej];"
      "W[bg];B[ji];W[dj];B[jh];W[ge];B[hg];W[ig])";
  } else if(boardSize == 9) {
    sgfData =
      "(;FF[4]GM[1]SZ[9]HA[0]KM[7];B[ef];W[ed];B[ge];W[gc];B[cc];W[cd];B[bd];W[ce];B[be];W[dg];B[cf];W[df];B[de];W[dd];"
      "B[ee];W[cg];B[bf];W[cb];B[eg];W[bc];B[bh];W[he];B[hd];W[gf];B[fe];W[hf];B[fc];W[eb];B[gd];W[fh];B[eh];W[hh];B["
      "ac];W[dc];B[fb];W[ab];B[fg];W[gg];B[fi];W[bg];B[dh];W[gh];B[ea];W[da];B[fa];W[ad];B[ch];W[id];B[ic];W[ie];B[gb];"
      "W[gi];B[ec];W[hc];B[hb];W[ei];B[db];W[ae];B[ag];W[eb];B[ig];W[db];B[ih];W[ii];B[di];W[ac];B[fi];W[hg];B[ei];W["
      "af];B[ff];W[if];B[fd];W[bb])";
  } else if(boardSize == 8) {
    sgfData =
      "(;FF[4]GM[1]SZ[8]HA[0]KM[10];B[ee];W[dd];B[ed];W[de];B[df];W[cf];B[dc];W[cc];B[ec];W[ef];B[dg];W[cg];B[eg];W[ff]"
      ";B[fg];W[cb];B[db];W[fe];B[ce];W[cd];B[be];W[fc];B[fd];W[gd];B[gc];W[da];B[fb];W[bf];B[bd];W[bc];B[ea];W[gg];B["
      "ca];W[ad];B[gf];W[ge];B[hg];W[hf];B[gh];W[ba];B[gf];W[dh];B[he];W[eh];B[fh];W[ch];B[da];W[ae];B[hd])";
  } else if(boardSize == 7) {
    sgfData =
      "(;FF[4]GM[1]SZ[7]HA[0]KM[9];B[dd];W[ed];B[ee];W[dc];B[cd];W[ec];B[fe];W[cc];B[bc];W[bb];B[bd];W[de];B[fd];W[df];"
      "B[ab];W[bf];B[ef];W[fc];B[cb];W[gd];B[dg];W[cg];B[cf];W[ce];B[be];W[eg];B[fg];W[ba];B[dg];W[gc];B[cf];W[db];B["
      "ac];W[ge];B[gf];W[ca];B[aa])";
  } else {
    throw StringError("getBenchmarkSGFData: unsupported board size: " + Global::intToString(boardSize));
  }
  return sgfData;
}

void TestCommon::overrideForBackends(bool& inputsNHWC, bool& useNHWC) {
#if defined(USE_OPENCL_BACKEND)
  if(inputsNHWC != false) {
    cout << "Backend is opencl, ignoring args and forcing inputsNHWC=false" << endl;
    inputsNHWC = false;
  }
  if(useNHWC != false) {
    cout << "Backend is opencl, ignoring args and forcing useNHWC=false" << endl;
    useNHWC = false;
  }
#elif defined(USE_TENSORRT_BACKEND)
  if(inputsNHWC != false) {
    cout << "Backend is TensorRT, ignoring args and forcing inputsNHWC=false" << endl;
    inputsNHWC = false;
  }
  if(useNHWC != false) {
    cout << "Backend is TensorRT, ignoring args and forcing useNHWC=false" << endl;
    useNHWC = false;
  }
#else
  (void)inputsNHWC;
  (void)useNHWC;
#endif
}

std::vector<string> TestCommon::getMultiGameSize9Data() {
  std::vector<string> sgfs(
    {"(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[7.5]RU[koPOSITIONALscoreAREAtaxALLsui1];B[ee];W[ec];B[gd];W[dg];B[fg];W["
     "ff];B[ef];W[eg];B[gg];W[cf];B[ed];W[gc];B[dc];W[fc];B[be];W[bf];B[eb];W[hd];B[ge];W[he];B[hf];W[fb];B[db];W[hb];"
     "B[ce];W[ae];B[bh];W[ch];B[eh];W[dh];B[fh];W[bd];B[cd];W[bc];B[bb];W[ab];B[cc];W[ah];B[bg];W[bi];B[ag];W[af];B[ai]"
     ";W[di];B[ci];W[cg];B[df];W[bi];B[ad];W[ac];B[ci];W[if];B[ig];W[bi];B[ga];W[ah];B[fa];W[ha];B[ie];W[ic];B[fd];W["
     "gb];B[ea];W[id];B[ba];W[if];B[ei];W[ie];B[gf];W[ad];)",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[7.5]RU[koSITUATIONALscoreAREAtaxALLsui0];B[ee];W[eg];B[cf];W[fc];B[dc];"
     "W[fe];B[ff];W[ef];B[gf];W[gh];B[hh];W[fh];B[de];W[gd];B[hg];W[he];B[hf];W[cg];B[bg];W[ge];B[dg];W[hi];B[ih];W[dh]"
     ";B[ch];W[db];B[eb];W[ec];B[cb];W[dd];B[da];W[df];B[ed];W[cg];B[ce];W[if];B[hc];W[gc];B[hb];W[fg];B[fb];W[gg];B["
     "dg];)",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[6.5]RU[koPOSITIONALscoreTERRITORYtaxNONEsui0];B[ee];W[ec];B[fc];W[fd];B["
     "ed];W[gc];B[fb];W[dc];B[cd];W[gd];B[cc];W[gb];B[fh];W[dh];B[eh];W[cf];B[dg];W[cg];B[ch];W[df];B[di];W[ef];B[fe];"
     "W[ff];B[ge];W[gf];B[he];W[hf];B[dd];W[eb];B[be];W[hh];B[bh];W[gh];B[fg];W[bf];B[db];W[fa];B[ae];W[af];B[eg];W[gg]"
     ";B[ig];W[if];B[gi];W[hi];B[ih];W[ce];B[bd];W[cb];B[bb];W[hd];B[ie];)",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[7]RU[koPOSITIONALscoreTERRITORYtaxALLsui1];B[ee];W[eg];B[dc];W[gf];B[ff]"
     ";W[fg];B[ge];W[hf];B[df];W[db];B[eb];W[cc];B[cb];W[dd];B[ec];W[ch];B[cg];W[bh];B[he];W[bb];B[da];W[bd];B[ce];W["
     "ae];B[be];W[cd];B[dh];W[ef];B[fe];W[de];B[dg];W[eh];B[ei];W[gh];B[af];W[bg];B[ad];W[bc];B[fi];W[bf];B[hh];W[cf];"
     ")",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[7]RU[koSITUATIONALscoreAREAtaxNONEsui0];B[df];W[dd];B[fe];W[ec];B[gd];W["
     "fg];B[eh];W[gf];B[fh];W[hd];B[hc];W[he];B[fc];W[gh];B[cc];W[cd];B[eb];W[cf];B[cg];W[dc];B[db];W[cb];B[be];W[bc];"
     "B[eg];W[bf];B[id];W[ie];B[hh];W[hi];B[hg];W[gg];B[de];W[gb];B[fb];W[hb];B[ce];W[bd];B[gc];W[ic];B[ib];W[ia];B[ga]"
     ";W[ih];B[ig];W[gi];B[ii];W[ee];B[ef];W[ih];B[ba];W[ca];B[ii];W[ed];B[ff];W[ih];B[bb];W[ab];B[ii];W[ge];B[fa];W["
     "ae];B[ag];W[id];B[ha];W[ib];B[fd];W[af];B[bg];W[ad];B[da];W[aa];B[fi];W[bb];)",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[7]RU[koSITUATIONALscoreTERRITORYtaxALLsui0];B[ee];W[ce];B[gf];W[dc];B["
     "ec];W[eb];B[dd];W[fc];B[ed];W[dg];B[db];W[fb];B[eh];W[dh];B[be];W[bf];B[bd];W[gd];B[fg];W[cf];B[bh];W[bg];B[ge];"
     "W[ch];B[cc];W[he];B[hf];W[ie];B[hc];W[hd];B[hb];W[if];B[hh];W[ig];B[ea];W[ha];B[fa];W[ga];B[da];W[gb];B[ic];W[fd]"
     ";B[fe];W[hg];B[gh];W[ei];B[fi];W[di];B[ih];W[eg];B[fh];W[ef];B[de];W[df];B[af];W[ag];B[ae];W[ff];B[gg];W[cd];B["
     "bc];W[];B[ia];W[];B[bi];W[ah];B[];W[];B[ib];W[];)",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[6.5]RU[koSITUATIONALscoreTERRITORYtaxSEKIsui0];B[ee];W[ge];B[cf];W[dc];"
     "B[fc];W[cd];B[gf];W[hf];B[gg];W[fe];B[ef];W[ed];B[hd];W[he];B[fd];W[gd];B[gc];W[hc];B[hb];W[id];B[ib];W[eb];B[ic]"
     ";W[ec];B[gb];W[hg];B[hh];W[fb];B[ff];W[gh];B[ih];W[fh];B[eh];W[hi];B[hd];W[dg];B[ie];W[eg];B[df];W[cg];B[bg];W["
     "bh];B[be];W[ag];B[bc];W[bd];B[ad];W[bb];B[ab];W[cc];B[ac];W[ba];B[bf];W[de];B[ce];W[aa];B[ah];W[ai];B[ae];W[fg];"
     "B[dd];W[da];B[ig];W[fa];B[gi];W[fi];B[ga];W[ch];)",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[6]RU[koSIMPLEscoreTERRITORYtaxNONEsui1];B[ee];W[ec];B[dc];W[dd];B[ed];W["
     "cc];B[db];W[fc];B[gd];W[cd];B[gc];W[cb];B[eg];W[cg];B[fb];W[dh];B[cf];W[eh];B[eb];W[df];B[fg];W[fh];B[dg];W[de];"
     "B[ch];W[bg];B[bh];W[bf];B[gh];W[gg];B[gf];W[hg];B[hh];W[hf];B[he];W[ff];B[ef];W[ih];B[ig];W[if];B[hi];W[fi];B[di]"
     ";W[ge];B[hd];W[ah];B[ci];W[da];B[ea];W[ca];B[af];W[be];B[fd];W[bi];B[ai];W[ag];B[fe];W[bi];)",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[7]RU[koSIMPLEscoreAREAtaxSEKIsui0];B[dd];W[ff];B[fe];W[ge];B[gd];W[gf];"
     "B[cf];W[hd];B[eg];W[fd];B[ec];W[fb];B[eb];W[eh];B[dh];W[fh];B[fc];W[gc];B[fg];W[gh];B[dg];W[gb];B[ee];W[ea];B[da]"
     ";W[fa];B[gg];W[hg];B[db];W[di];B[ci];W[ei];B[ch];W[ef];B[df];W[];B[de];W[];B[ed];W[gd];B[];W[])",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[7]RU[koSITUATIONALscoreAREAtaxALLsui1button1];B[ee];W[ce];B[gf];W[ge];B["
     "ff];W[cg];B[ec];W[fd];B[fc];W[gc];B[cd];W[bd];B[gb];W[eg];B[cc];W[hb];B[hc];W[hd];B[gd];W[gh];B[gc];W[hg];B[df];"
     "W[cf];B[dg];W[dh];B[ch];W[eh];B[bc];W[hf];B[bh];W[de];B[ef];W[ae];B[bg];W[bf];B[dd];W[bi];B[ci];W[ag];B[ac];W[ah]"
     ";B[ai];)",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[6.5]RU[koPOSITIONALscoreTERRITORYtaxNONEsui0];B[ee];W[ec];B[eg];W[cd];B["
     "fc];W[fb];B[fd];W[eh];B[dh];W[fg];B[fh];W[ef];B[dg];W[ff];B[df];W[gh];B[ei];W[ge];B[eb];W[db];B[dc];W[ea];B[cc];"
     "W[dd];B[ed];W[eb];B[hd];W[he];B[gc];W[bc];B[hh];W[gi];B[hf];W[hg];B[ie];W[ih];B[hi];W[de];B[if];W[cf];B[bg];W[bf]"
     ";B[gb];W[ga];B[ha];W[hb];B[fa];W[cg];B[ch];W[bh];B[fi];W[ga];B[cb];W[gg];B[bi];W[ia];B[ic];W[bb];B[ib];W[ah];B["
     "ci];W[ha];B[hc];W[ca];B[fa];W[ha];B[ig];W[ga];B[fe];W[hb];B[gf];W[fa];B[ia];)",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[6.5]RU[koSITUATIONALscoreTERRITORYtaxSEKIsui0];B[ee];W[ce];B[gf];W[fc];"
     "B[dc];W[gd];B[cf];W[bf];B[cg];W[de];B[ef];W[ed];B[dd];W[bd];B[hc];W[hd];B[fb];W[gb];B[ec];W[fd];B[eb];W[cb];B[hb]"
     ";W[hf];B[hg];W[gc];B[ga];W[he];B[ha];W[fa];B[fe];W[da];B[ea];W[gg];B[gh];W[fg];B[hh];W[fh];B[eg];W[ih];B[eh];W["
     "ic];B[ib];W[cc];B[id];W[hi];B[fi];W[bg];B[bh];)",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[7.5]RU[koSITUATIONALscoreAREAtaxNONEsui0];B[ee];W[ec];B[cd];W[fg];B[dg];"
     "W[df];B[ef];W[eg];B[dh];W[cf];B[cg];W[gf];B[fd];W[bg];B[bh];W[cc];B[de];W[eh];B[bc];W[di];B[ch];W[be];B[ce];W[bf]"
     ";B[ah];W[gc];B[gd];W[hc];B[hd];W[db];B[bd];W[hf];B[cb];W[dc];B[ba];W[ic];B[da];W[ea];B[ie];W[if];B[fc];W[fb];B["
     "ca];W[hb];B[ci];W[ei];)",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[7]RU[koSIMPLEscoreAREAtaxSEKIsui0];B[dd];W[ff];B[fe];W[ge];B[gd];W[gf];"
     "B[cf];W[hd];B[eg];W[fd];B[ec];W[fb];B[eb];W[eh];B[dh];W[fh];B[fc];W[gc];B[fg];W[gg];B[dg];W[gb];B[ee];W[ef];B[df]"
     ";W[ea];B[da];W[fa];B[dc];W[ei];B[di];W[gh];B[db];W[gd];B[ed];W[];B[de];W[];B[hb];W[ha];B[hf];W[hc];B[];W[he];B[];"
     "W[ib];B[];W[hg];B[];W[if];B[];W[])",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[7]RU[koSIMPLEscoreAREAtaxSEKIsui1];B[dd];W[ff];B[ef];W[eg];B[dg];W[fg];"
     "B[fc];W[cd];B[ce];W[gd];B[fd];W[dc];B[ee];W[dh];B[ge];W[cg];B[bd];W[gc];B[hd];W[hc];B[hf];W[fb];B[eb];W[fa];B[fe]"
     ";W[id];B[he];W[ec];B[db];W[ha];B[gb];W[ic];B[gh];W[gg];B[hh];W[bf];B[fh];W[cc];B[be];W[bc];B[bb];W[cb];B[bh];W["
     "df];B[cf];W[bg];B[ah];W[hg];B[ig];W[eh];B[ch];W[ag];B[de];W[dg];B[ei];W[bi];B[ac];W[ab];B[aa];W[ca];B[fi];W[ih];"
     "B[ii];W[ba];B[di];W[ci];B[gf];W[ai];B[bh];W[ab];B[ae];W[ed];B[if];)",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[6]RU[koSIMPLEscoreTERRITORYtaxNONEsui1];B[ff];W[dd];B[de];W[ce];B[cf];W["
     "cd];B[gd];W[bf];B[ec];W[cg];B[ed];W[fg];B[df];W[dg];B[bg];W[be];B[gg];W[fh];B[gh];W[ef];B[ee];W[eg];B[cb];W[gf];"
     "B[hf];W[fe];B[ge];W[ff];B[fd];W[bb];B[bh];W[ch];B[ei];W[di];B[fi];W[eh];B[gi];W[bi];B[cc];W[dc];B[db];W[he];B[hg]"
     ";W[hd];B[gc];W[hc];B[hb];W[fb];B[eb];W[bc];B[ba];)",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[6]RU[koSIMPLEscoreTERRITORYtaxNONEsui0];B[ee];W[ec];B[dc];W[dd];B[ed];W["
     "cc];B[db];W[fc];B[gd];W[cd];B[gc];W[cb];B[dg];W[eh];B[gg];W[cg];B[dh];W[df];B[ch];W[ef];B[eg];W[ff];B[gf];W[bg];"
     "B[gb];W[fg];B[fh];W[bh];B[eb];W[gh];B[ei];W[hg];B[hf];W[hi];B[cf];W[bf];B[ig];W[fi];B[gi];W[fb];B[fd];W[fi];B[de]"
     ";W[ce];B[gi];W[fa];B[fe];W[cf];B[hh];W[da];)",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[7.5]RU[koPOSITIONALscoreAREAtaxALLsui0button1];B[ee];W[eg];B[dc];W[gf];"
     "B[ge];W[ff];B[fe];W[df];B[be];W[he];B[hd];W[hf];B[cg];W[de];B[cd];W[bf];B[bg];W[gc];B[hc];W[cf];B[af];W[fc];B[ed]"
     ";W[ec];B[dd];W[db];B[cb];W[hb];B[gd];W[da];B[ha];W[ib];B[fa];W[fb];B[ca];W[eb];B[gb];W[ga];B[fh];W[eh];B[gb];W["
     "ae];B[ad];W[ga];B[dh];W[gb];B[ef];W[fg];B[dg];W[di];B[ci];W[ei];B[ie];W[if];B[id];)",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[6.5]RU[koSITUATIONALscoreTERRITORYtaxSEKIsui1];B[ee];W[ce];B[dc];W[gf];"
     "B[eg];W[fd];B[cg];W[bg];B[bh];W[cf];B[bc];W[fe];B[dg];W[ed];B[de];W[dd];B[cd];W[eb];B[db];W[fg];B[eh];W[be];B[bd]"
     ";W[ch];B[ci];W[ah];B[bi];W[fh];B[fi];W[ei];B[di];W[gi];B[fb];W[ec];B[ea];W[ei];B[ef];W[gb];B[ga];W[ha];B[fa];W["
     "fc];B[da];W[hc];B[ae];W[ff];B[fi];W[bb];B[ei];W[af];B[ab];W[ad];B[ac];W[ae];B[ai];W[ag];B[gh];W[ca];B[cc];W[aa];"
     "B[ba];W[hi];B[cb];W[hh];)",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[6]RU[koPOSITIONALscoreTERRITORYtaxSEKIsui1];B[ee];W[ge];B[gd];W[cd];B["
     "dc];W[hd];B[gc];W[cf];B[df];W[gg];B[cg];W[be];B[fh];W[hc];B[bf];W[ce];B[bg];W[cc];B[cb];W[bb];B[db];W[ba];B[gh];"
     "W[hb];B[fg];W[gf];B[gb];W[fd];B[fc];W[ga];B[fa];W[hh];B[hi];W[ed];B[dd];W[ec];B[ha];W[fe];B[de];W[ae];B[hg];W[eb]"
     ";B[ea];)",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[7]RU[koSIMPLEscoreAREAtaxSEKIsui0];B[df];W[ff];B[fe];W[ge];B[fd];W[gd];"
     "B[gf];W[fg];B[gg];W[gh];B[hh];W[fh];B[fc];W[he];B[cf];W[dh];B[ch];W[gc];B[gb];W[hb];B[fb];W[cd];B[hg];W[dc];B[hi]"
     ";W[bh];B[cg];W[ci];B[ef];W[eh];B[bg];W[ee];B[ha];W[ib];B[dd];W[ed];B[ec];W[de];B[ce];W[dd];B[db];W[bd];B[bb];W["
     "bi];B[ah];W[be];B[di];W[ei];B[bc];W[eg];B[af];W[gi];B[ad];W[hf];B[ag];)",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[6]RU[koSITUATIONALscoreTERRITORYtaxSEKIsui1];B[ee];W[ce];B[ge];W[he];B["
     "gd];W[gf];B[cf];W[hd];B[gc];W[df];B[de];W[bf];B[cg];W[cd];B[dc];W[dd];B[ff];W[gg];B[bg];W[ec];B[cc];W[eb];B[bb];"
     "W[ed];B[bd];W[be];B[db];W[dg];B[dh];W[ad];B[bc];W[eg];B[fg];W[eh];B[fh];W[ch];B[bh];W[di];B[bi];W[ah];B[fi];W[ei]"
     ";B[ci];W[ag];B[dh];W[hc];B[hb];W[ea];B[gb];W[ch];B[hh];W[ca];B[dh];W[gh];B[ef];W[gi];B[da];W[ba];B[ac];)",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[6.5]RU[koPOSITIONALscoreTERRITORYtaxSEKIsui0];B[ee];W[ce];B[cd];W[dd];B["
     "de];W[cc];B[bd];W[cf];B[dg];W[dc];B[cg];W[bc];B[hd];W[ge];B[he];W[gg];B[gd];W[eh];B[ih];W[hg];B[eg];W[dh];B[ch];"
     "W[fg];B[di];W[fi];B[ig];W[fe];B[fd];W[hf];B[if];W[ff];B[bg];W[be];B[ci];W[hh];B[ed];W[hi];B[eb];W[ec];)",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[7]RU[koSIMPLEscoreTERRITORYtaxALLsui0];B[ee];W[ge];B[cd];W[fg];B[ff];W["
     "gf];B[eg];W[fh];B[fd];W[gd];B[fc];W[eh];B[dg];W[bd];B[be];W[cc];B[bc];W[gc];B[gb];W[hb];B[fb];W[ch];B[cg];W[bh];"
     "B[bg];W[ah];B[ha];W[ib];B[dh];W[di];B[fe];W[ag];B[af];W[hc];B[ga];W[ia];B[];W[];B[];W[];B[ad];W[];B[dc];W[];B[ef]"
     ";W[];B[dd];W[];B[cb];W[];B[de];W[];B[ce];W[];B[])",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[7]RU[koSIMPLEscoreAREAtaxSEKIsui1];B[fd];W[df];B[ef];W[eg];B[fg];W[dg];"
     "B[dc];W[fh];B[ce];W[gg];B[ff];W[gf];B[ge];W[ee];B[fe];W[de];B[gh];W[hh];B[cd];W[he];B[hf];W[hg];B[hd];W[ed];B[ec]"
     ";W[if];B[gc];W[bf];B[cf];W[cg];B[be];W[bg];B[ae];W[id];B[ic];W[ie];B[af];W[ag];B[dd];W[gi];B[];W[])",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[7]RU[koSIMPLEscoreAREAtaxALLsui0];B[ee];W[eg];B[fc];W[cf];B[df];W[dg];B["
     "ce];W[bf];B[ff];W[fg];B[gf];W[be];B[cc];W[fb];B[gc];W[bc];B[cd];W[bb];B[gg];W[cb];B[db];W[da];B[eb];W[ea];B[bd];"
     "W[ad];B[fh];W[eh];B[gh];W[de];B[dd];W[gb];B[hb];W[ha];B[ic];W[ef];B[de];W[ec];B[ei];W[ch];B[dc];W[di];B[fa];W[ga]"
     ";B[ia];W[ib];B[cg];W[hc];B[dh];W[bg];B[dg];W[he];B[bh];W[ah];B[ci];W[ih];B[ge];W[hd];B[ai];W[bi];B[hf];W[ch];B["
     "ae];W[af];B[bh];W[if];B[ai];W[ac];B[ag];W[ae];B[gd];W[ah];B[ie];W[id];B[ag];W[hh];B[hi];W[ah];B[hg];W[ig];B[ag];"
     "W[gi];B[ah];)",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[6.5]RU[koSITUATIONALscoreTERRITORYtaxSEKIsui0];B[ee];W[ce];B[gf];W[fc];"
     "B[dc];W[gd];B[cf];W[bf];B[cg];W[de];B[ef];W[ed];B[dd];W[bd];B[hc];W[hd];B[fb];W[gb];B[ec];W[fd];B[eb];W[cb];B[hb]"
     ";W[gc];B[ga];W[hf];B[hg];W[he];B[ha];W[fa];B[fe];W[da];B[ea];W[gg];B[fg];W[df];B[gh];W[cc];B[if];)",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[6.5]RU[koSIMPLEscoreTERRITORYtaxNONEsui1];B[ee];W[ce];B[dc];W[gf];B[eg];"
     "W[fd];B[cg];W[bg];B[bh];W[cf];B[dh];W[bc];B[bb];W[cb];B[cc];W[ba];B[ab];W[ac];B[gc];W[db];B[fc];W[ed];B[he];W[ge]"
     ";B[ec];W[hd];B[hc];W[gd];B[gh];W[de];B[eb];W[hh];B[hg];W[gg];B[ih];W[hi];B[gi];W[ef];B[hf];W[ic];B[ib];W[id];B["
     "ha];W[dg];B[df];W[fh];B[ii];W[dg];B[bd];W[ch];B[ad];W[ie];B[if];W[ei];B[fi];W[eh];B[af];W[be];B[ae];W[ag];B[bf];"
     "W[dd];B[cd];W[];B[];W[];B[];W[fe];B[ca];W[cg];)",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[6.5]RU[koPOSITIONALscoreAREAtaxSEKIsui1button1];B[fd];W[df];B[de];W[ce];"
     "B[cd];W[cf];B[gf];W[bd];B[eg];W[cc];B[dd];W[dc];B[ec];W[ee];B[ed];W[ef];B[bc];W[bb];B[fg];W[eb];B[fb];W[fe];B[db]"
     ";W[cb];B[ge];W[da];B[gc];W[dh];B[eh];W[ch];B[di];W[ci];B[ei];W[fa];B[ga];W[ea];B[dg];W[cg];B[ff];W[];B[];W[ac];B["
     "];W[])",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[6]RU[koSITUATIONALscoreTERRITORYtaxSEKIsui0];B[ee];W[ge];B[gd];W[fd];B["
     "fe];W[gc];B[hd];W[gf];B[fg];W[fc];B[gg];W[hc];B[bd];W[ce];B[be];W[cg];B[bf];W[eh];B[bg];W[hg];B[ff];W[he];B[hh];"
     "W[dd];B[ed];W[dc];B[cf];W[ih];B[cb];W[db];B[gi];W[bb];B[cc];W[cd];B[bc];W[ec];B[if];W[hf];B[ig];W[ie];B[de];W[ca]"
     ";B[ig];W[if];B[ab];W[ba];B[ac];W[id];B[hi];W[dg];B[df];W[bh];B[eg];W[fi];B[gh];W[ah];B[bi];W[ci];B[dh];W[ch];B["
     "fh];W[di];B[ei];W[ig];)",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[6.5]RU[koPOSITIONALscoreAREAtaxNONEsui1button1];B[ee];W[ec];B[dg];W[gf];"
     "B[gd];W[fd];B[fe];W[ge];B[gc];W[fg];B[dd];W[dc];B[cc];W[cb];B[bb];W[fc];B[cd];W[fb];B[gb];W[hd];B[hc];W[he];B[ha]"
     ";W[ib];B[db];W[fa];B[eh];W[fh];B[fi];W[eg];B[dh];W[gi];B[ei];W[gh];B[ea];W[eb];B[ca];W[da];B[cb];W[ic];B[ef];W["
     "ga];B[ff];W[hb];)",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[7]RU[koSIMPLEscoreTERRITORYtaxALLsui1];B[ee];W[ec];B[dg];W[gd];B[ge];W["
     "fd];B[fe];W[dd];B[hd];W[hc];B[ce];W[cd];B[be];W[dh];B[ed];W[cg];B[fc];W[gc];B[dc];W[eb];B[eg];W[eh];B[db];W[fb];"
     "B[fh];W[df];B[ef];W[gh];B[fi];W[de];B[cf];W[bd];B[bg];W[cb];B[bb];W[cc];B[ad];W[hg];B[fg];W[he];B[bc];W[da];B[hf]"
     ";W[id];B[if];W[gf];B[gg];W[ch];B[bh];W[hh];B[gi];W[ie];B[hi];W[ig];)",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[6.5]RU[koPOSITIONALscoreAREAtaxSEKIsui1button1];B[ee];W[ec];B[dc];W[dd];"
     "B[ed];W[cc];B[db];W[fc];B[gd];W[cd];B[gc];W[cb];B[dg];W[eh];B[gg];W[cg];B[dh];W[df];B[eg];W[ch];B[ci];W[bi];B[di]"
     ";W[gb];B[hb];W[fb];B[ha];W[fd];B[fe];W[bh];B[ga];W[ef];B[ff];W[da];B[cf];W[bf];B[de];W[ce];B[fa];)",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[7]RU[koSIMPLEscoreAREAtaxNONEsui1];B[dd];W[df];B[ef];W[eg];B[ff];W[fg];"
     "B[dg];W[cf];B[cg];W[bg];B[bh];W[bf];B[gf];W[eh];B[dc];W[bd];B[bc];W[gc];B[ch];W[eb];B[ah];W[gg];B[hg];W[hh];B[he]"
     ";W[ig];B[hf];W[be];B[de];W[cb];B[cc];W[db];B[hc];W[bb];B[ad];W[dh];B[ag];W[gb];B[ih];W[ii];B[if];W[gi];B[ih];W["
     "hb];B[hi];W[ci];B[af];W[ii];B[fc];W[fb];B[hi];W[ae];B[ce];W[ii];B[ig];W[fh];B[hi];W[ac];B[cd];W[ii];B[gd];W[hi];"
     "B[ib];W[ec];B[fd];W[ed];B[ad];)",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[7]RU[koSITUATIONALscoreAREAtaxNONEsui0];B[df];W[fd];B[fe];W[ge];B[gf];W["
     "gd];B[cd];W[hf];B[ec];W[gg];B[ff];W[ed];B[dd];W[fg];B[eg];W[ee];B[gb];W[de];B[ce];W[cf];B[cg];W[dc];B[eb];W[bf];"
     "B[bg];W[cc];B[be];W[ef];B[af];W[hb];B[ha];W[db];B[bc];W[da];B[fb];W[bb];B[ab];W[hc];B[eh];W[ca];B[ba];W[aa];B[ac]"
     ";W[gc];B[ba];W[ga];B[cb];W[fh];B[fa];W[bb];B[ei];W[aa];B[cf];W[fi];B[ba];W[fc];B[cb];W[ia];B[ea];W[ga];B[ca];W["
     "ha];B[];W[])",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[6.5]RU[koSIMPLEscoreTERRITORYtaxSEKIsui0];B[ee];W[eg];B[dg];W[df];B[ef];"
     "W[cg];B[dh];W[fg];B[gf];W[cf];B[gg];W[ch];B[db];W[ec];B[eb];W[gc];B[fb];W[he];B[gb];W[gh];B[cd];W[dd];B[ff];W[fh]"
     ";B[hh];W[ed];B[gi];W[di];B[fi];W[eh];B[de];W[ce];B[gd];W[fd];B[ge];W[cc];B[bd];W[bc];)",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[6.5]RU[koPOSITIONALscoreTERRITORYtaxNONEsui1];B[ee];W[eg];B[dg];W[dh];B["
     "cg];W[ec];B[dc];W[ef];B[de];W[db];B[cc];W[gf];B[ge];W[gc];B[hf];W[gg];B[fe];W[cb];B[hg];W[ch];B[bh];W[bg];B[bf];"
     "W[hh];B[ih];W[bi];B[ag];W[hd];B[he];W[fh];B[ff];W[fg];B[hi];W[ei];B[ci];W[gi];B[gh];)",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[7]RU[koPOSITIONALscoreTERRITORYtaxALLsui1];B[ee];W[eg];B[dc];W[gf];B[ff]"
     ";W[fg];B[ge];W[hf];B[df];W[db];B[eb];W[cc];B[cb];W[dd];B[ec];W[bb];B[da];W[ch];B[bg];W[gd];B[cg];W[bh];B[dg];W["
     "dh];B[he];W[fd];B[fe];W[ed];B[gg];W[bd];B[gh];W[hd];B[id];W[ag];B[af];W[ah];B[be];W[bf];B[cf];W[fh];B[gb];W[hb];"
     "B[ba];W[ic];B[hg];W[gc];B[fb];W[ha];B[ab];W[ac];B[bc];W[ie];B[if];W[bb];B[cd];W[ce];B[bc];W[hf];B[cd];W[ig];B[de]"
     ";)",
     "(;FF[4]GM[1]SZ[9]PB[bot0]PW[bot1]HA[0]KM[6.5]RU[koPOSITIONALscoreTERRITORYtaxNONEsui0];B[ee];W[ge];B[gf];W[ff];B["
     "fe];W[gg];B[hf];W[gd];B[fc];W[fg];B[gc];W[hg];B[bf];W[bd];B[be];W[dd];B[dc];W[de];B[df];W[ef];B[ed];W[dg];B[cf];"
     "W[bh];B[ch];W[cg];B[bg];W[dh];B[ah];W[ci];B[cd];W[hc];B[hb];W[ic];B[fd];W[he];B[ib];)",
     "(;FF[4]GM[1]SZ[9]PB[bot1]PW[bot0]HA[0]KM[7]RU[koPOSITIONALscoreAREAtaxSEKIsui0button1];B[ee];W[ec];B[eg];W[cd];B["
     "fc];W[fb];B[fd];W[eh];B[dh];W[fg];B[fh];W[ef];B[ei];W[df];B[ff];W[gb];B[de];W[ce];B[cf];W[bf];B[cg];W[hd];B[he];"
     "W[hc];B[bg];W[be];B[dc];W[dd];B[ed];W[eb];B[cc];W[bc];B[af];W[hf];B[ge];W[ie];B[ae];W[bd];B[hg];W[ad];B[ag];W[cb]"
     ";)"});
  return sgfs;
}

std::vector<string> TestCommon::getMultiGameSize13Data() {
  std::vector<string> sgfs(
    {"(;FF[4]GM[1]SZ[13]PB[bot1]PW[bot0]HA[0]KM[7.5]RU[koSIMPLEscoreAREAtaxSEKIsui1];B[jd];W[dc];B[kj];W[dk];B[ij];W["
     "ce];B[fk];W[di];B[hc];W[lj];B[li];W[fi];B[eh];W[ei];B[ch];W[kk];B[jj];W[hk];B[il];W[hl];B[hj];W[dh];B[cf];W[de];"
     "B[ef];W[gd];B[gf];W[hd];B[gc];W[id];B[ic];W[fd];B[bi];W[kd];B[jc];W[je];B[ke];W[bk];B[be];W[bd];B[bg];W[jf];B[cj]"
     ";W[kc];B[le];W[jb];B[kb];W[lb];B[ka];W[ld];B[md];W[lc];B[ib];W[lf];B[ck];W[ek];B[cl];W[fl];B[ik];W[gj];B[eb];W["
     "ll];B[lg];W[kf];B[jl];W[kh];B[ki];W[lh];B[mh];W[fb];B[fc];W[ec];B[db];W[cb];B[gb];W[ae];B[cc];W[bb];B[ed];W[dd];"
     "B[ee];W[bf];B[ig];W[kg];B[ie];W[if];B[he];W[hf];B[ge];W[hg];B[dl];W[hi];B[el];W[im];B[mj];W[lk];B[ii];W[ih];B[mi]"
     ";W[hm];B[kl];W[lm];B[mk];W[mg];B[gl];W[gk];B[ml];W[jh];B[fm];W[la];B[gh];W[gg];B[fh];W[eg];B[fg];W[dg];B[ja];W["
     "ji];B[df];W[cg];B[ag];W[ci];)",
     "(;FF[4]GM[1]SZ[13]PB[bot0]PW[bot1]HA[0]KM[7]RU[koSITUATIONALscoreAREAtaxNONEsui1button1];B[dd];W[jj];B[jc];W[dj];"
     "B[kh];W[cf];B[dk];W[jd];B[kd];W[ck];B[ek];W[cj];B[hk];W[il];B[hl];W[ik];B[hj];W[ic];B[kc];W[fc];B[ec];W[ee];B[fb]"
     ";W[gb];B[fd];W[gc];B[je];W[ed];B[eb];W[ge];B[be];W[de];B[cd];W[ki];B[ff];W[fe];B[lh];W[jh];B[jg];W[ih];B[ej];W["
     "bf];B[lj];W[ig];B[jf];W[gi];B[gh];W[bc];B[bd];W[cb];B[db];W[hi];B[ij];W[lk];B[fi];W[li];B[ii];W[ji];B[kl];W[jk];"
     "B[mi];W[hh];B[gj];W[cl];B[hm];W[im];B[dl];W[eh];B[ei];W[da];B[ea];W[fh];B[di];W[bh];B[ba];W[fl];B[dm];W[id];B[dh]"
     ";W[dg];B[kk];W[kj];B[mj];W[ll];B[cm];W[bl];B[ib];W[hb];B[ia];W[ch];B[ml];W[km];B[mk];W[jl];B[fk];W[bb];B[ad];W["
     "ca];B[ac];W[fa];B[ga];W[ha];B[jb];W[ie];B[ci];W[bi];B[bm];W[fa];B[dc];W[if];B[kg];W[al];B[af];)",
     "(;FF[4]GM[1]SZ[13]PB[bot0]PW[bot1]HA[0]KM[7.5]RU[koSIMPLEscoreAREAtaxSEKIsui0button1];B[dd];W[jc];B[cj];W[jk];B["
     "jd];W[kc];B[jj];W[kk];B[hd];W[id];B[ie];W[ic];B[hj];W[ij];B[ii];W[ik];B[hg];W[dj];B[ci];W[dk];B[di];W[ck];B[hc];"
     "W[ei];B[eh];W[fh];B[fi];W[ej];B[eg];W[cg];B[cf];W[je];B[hk];W[cc];B[cd];W[dc];B[el];W[df];B[ef];W[bd];B[be];W[ed]"
     ";B[ee];W[bf];B[bc];W[ce];B[ad];W[bh];B[dh];W[bj];B[bi];W[gi];B[hi];W[ai];B[bk];W[aj];B[bl];W[de];B[bd];W[dg];B["
     "fj];W[fk];B[gj];W[fl];B[dl];W[fd];B[cb];W[db];B[bb];W[hb];B[ag];W[af];B[ah];W[bg];B[ah];W[ca];B[fb];W[ec];B[ba];"
     "W[ab];B[da];W[ea];B[fe];W[he];B[ge];W[gc];B[gd];W[gb];B[fc];W[ca];B[il];W[fa];B[da];W[eb];B[ki];W[hl];B[gl];W[jl]"
     ";B[hm];)",
     "(;FF[4]GM[1]SZ[13]PB[bot1]PW[bot0]HA[0]KM[7.5]RU[koPOSITIONALscoreAREAtaxNONEsui0button1];B[jj];W[kd];B[dk];W[cd]"
     ";B[di];W[kh];B[ed];W[ec];B[jd];W[ke];B[fc];W[dc];B[jc];W[kc];B[fd];W[cf];B[jh];W[jg];B[ki];W[ih];B[ji];W[hc];B["
     "ig];W[ie];B[hg];W[il];B[jl];W[jk];B[kk];W[ik];B[kl];W[gf];B[dd];W[cc];B[hd];W[ej];B[ek];W[gg];B[hb];W[gb];B[fb];"
     "W[ib];B[jb];W[id];B[ic];W[ha];B[gc];W[kg];B[gh];W[he];B[hb];W[fj];B[fh];W[hc];B[gd];W[dj];B[cj];W[ei];B[ci];W[eh]"
     ";B[kb];W[hi];B[hj];W[gi];B[hh];W[lb];B[la];W[mb];B[ii];W[fk];B[cg];W[df];B[bg];W[je];B[cl];W[hb];B[eg];W[dg];B["
     "fg];W[dh];B[de];W[ce];B[ja];W[fe];B[eb];W[db];B[ea];W[ee];B[ga];W[fa];B[gl];W[fl];B[ga];W[ch];B[bh];W[fa];B[bj];"
     "W[gk];B[li];W[lh];B[mi];W[bf];B[mh];W[mg];B[ga];W[el];B[dl];W[fa];B[lf];W[lg];B[ga];W[af];B[ag];W[fa];B[lc];W[ge]"
     ";B[ga];W[bi];B[ia];W[ah];B[mc];W[ef];)",
     "(;FF[4]GM[1]SZ[13]PB[bot0]PW[bot1]HA[0]KM[7.5]RU[koSIMPLEscoreAREAtaxNONEsui0];B[dj];W[jk];B[cd];W[jc];B[jd];W["
     "kc];B[jj];W[kk];B[hj];W[dd];B[dc];W[ij];B[ii];W[ik];B[hd];W[id];B[ie];W[ic];B[de];W[ed];B[ee];W[cc];B[bc];W[cb];"
     "B[be];W[hi];B[fd];W[ih];B[el];W[ck];B[cj];W[bj];B[dk];W[he];B[if];W[gd];B[hf];W[hc];B[ge];W[gf];B[hd];W[cl];B[bi]"
     ";W[he];B[fe];W[gh];B[le];W[fb];B[eb];W[fc];B[ec];W[ak];B[kh];W[ki];B[lc];W[lb];B[kd];W[mc];B[lh];W[ld];B[ai];W["
     "dl];B[fk];W[bm];B[lj];W[ji];B[li];W[jg];B[ke];W[lf];B[kf];W[me];B[kg];W[jf];B[je];W[eh];B[lk];W[ci];B[di];W[ch];"
     "B[dh];W[bh];B[hk];W[ll];)",
     "(;FF[4]GM[1]SZ[13]PB[bot0]PW[bot1]HA[0]KM[7.5]RU[koSITUATIONALscoreAREAtaxNONEsui1];B[dd];W[jc];B[dj];W[jk];B[ji]"
     ";W[kf];B[gk];W[fd];B[kk];W[kl];B[kj];W[il];B[ij];W[cc];B[dc];W[cd];B[ce];W[be];B[bf];W[cf];B[de];W[bg];B[bd];W["
     "af];B[bc];W[cb];B[bb];W[ek];B[hl];W[dk];B[cj];W[ej];B[ck];W[ei];B[ch];W[df];B[ik];W[cl];B[bl];W[dl];B[bh];W[bm];"
     "B[eh];W[ee];B[db];W[dh];B[di];W[dg];B[al];W[gh];B[bj];W[lk];B[lj];W[mk];B[jm];W[lh];B[mj];W[ll];B[im];W[fb];B[ke]"
     ";W[je];B[kd];W[jd];B[kc];W[le];B[ld];W[lf];B[jb];W[lb];B[kb];W[ib];B[ic];W[hc];B[ia];W[hb];B[mb];W[la];B[md];W["
     "ih];B[hi];W[hh];B[kh];W[lc];B[ja];W[kg];B[jh];W[jg];B[fl];W[jl];B[gi];W[km];B[fi];W[fh];B[mm];W[hm];B[im];W[li];"
     "B[jj];W[gm];B[fj];W[el];B[fm];W[hj];B[gl];W[hm];B[ii];W[em];B[cm];W[ki];B[gm];W[mi];B[hm];W[dm];B[eb];W[ea];B[mc]"
     ";W[ha];B[fc];W[ed];B[ec];W[eg];B[jm];W[ah];B[ai];W[gc];B[ca];)",
     "(;FF[4]GM[1]SZ[13]PB[bot0]PW[bot1]HA[0]KM[7]RU[koSIMPLEscoreTERRITORYtaxNONEsui1];B[dj];W[jk];B[cd];W[jc];B[jd];"
     "W[kc];B[ji];W[dd];B[cc];W[kj];B[ik];W[il];B[hd];W[id];B[hl];W[ie];B[ij];W[jl];B[ki];W[ck];B[cj];W[dk];B[ej];W[fk]"
     ";B[fj];W[dc];B[de];W[ee];B[ed];W[eb];B[fd];W[ce];B[df];W[cb];B[be];W[cf];B[cg];W[ge];B[fe];W[bf];B[bg];W[fl];B["
     "hc];W[bd];B[af];W[bc];B[gk];W[he];B[fb];W[lf];B[ce];W[ba];B[kf];W[bj];B[le];W[ff];B[ef];W[ld];B[ke];W[ic];B[lb];"
     "W[md];B[ib];W[jb];B[ja];W[ka];B[ha];W[ia];B[lj];W[lk];B[li];W[ll];B[ja];W[hg];B[jj];W[im];B[kb];W[ia];B[kk];W[kl]"
     ";B[ja];W[kg];B[lg];W[ia];B[ek];W[el];B[ja];W[kd];B[jg];W[ia];B[ec];W[hb];B[db];W[ab];B[bi];W[bk];B[gb];W[ga];B["
     "fg];W[gh];B[gf];W[gg];B[gl];W[fm];B[fh];W[gi];B[je];W[mf];B[kh];W[me];)",
     "(;FF[4]GM[1]SZ[13]PB[bot1]PW[bot0]HA[0]KM[7]RU[koSITUATIONALscoreTERRITORYtaxSEKIsui0];B[jd];W[kj];B[dc];W[cj];B["
     "dj];W[ck];B[ij];W[dd];B[cd];W[ce];B[cc];W[de];B[ed];W[dh];B[kk];W[kc];B[kd];W[jc];B[id];W[lk];B[jk];W[hb];B[kh];"
     "W[fj];B[di];W[eh];B[ee];W[ci];B[gk];W[fb];B[cg];W[bg];B[gc];W[gb];B[ec];W[hh];B[hi];W[ld];B[le];W[lc];B[ih];W[kl]"
     ";B[ll];W[gj];B[hk];W[fk];B[gl];W[gh];B[hg];W[gg];B[me];W[bd];B[bc];W[be];B[el];W[fl];B[fm];W[dl];B[ek];W[hf];B["
     "ig];W[ef];B[ff];W[gf];B[eg];W[df];B[fg];W[fe];B[ge];W[dg];B[ic];W[ib];B[fd];W[dk];B[ej];W[eb];B[db];W[ei];B[em];"
     "W[ac];B[ab];W[ad];B[ba];W[fh];B[fe];W[dm];B[gm];W[if];B[jf];W[ie];B[md];W[mc];B[je];W[da];B[ca];W[ea];B[gi];W[fi]"
     ";B[hj];W[hd];B[hc];W[he];B[fc];W[gd];B[];W[];B[];W[];B[lj];W[ch];B[ki];W[];B[jj];W[cf];B[jl];W[];B[km];W[];B[mk];"
     "W[];B[])",
     "(;FF[4]GM[1]SZ[13]PB[bot1]PW[bot0]HA[0]KM[7.5]RU[koSITUATIONALscoreAREAtaxNONEsui1button1];B[jj];W[dk];B[kd];W["
     "dd];B[fc];W[jd];B[jc];W[ic];B[je];W[id];B[kc];W[gd];B[cc];W[cd];B[dc];W[fd];B[dj];W[ek];B[ed];W[ee];B[ec];W[bc];"
     "B[bb];W[be];B[ac];W[jg];B[ef];W[fe];B[cf];W[de];B[cj];W[ck];B[fi];W[hk];B[if];W[df];B[dg];W[hg];B[ig];W[ih];B[hh]"
     ";W[ii];B[hf];W[cg];B[gg];W[kf];B[ki];W[ij];B[jk];W[kh];B[ke];W[dh];B[eg];W[bh];B[ej];W[il];B[bi];W[eh];B[fh];W["
     "bf];B[jl];W[li];B[lj];W[ji];B[kj];W[bk];B[gl];W[gj];B[fj];W[fk];B[hl];W[ik];B[im];W[fl];B[lh];W[lg];B[mi];W[le];"
     "B[ld];W[bd];B[gc];W[ib];B[jb];W[ha];B[ca];W[mf];B[gb];W[gk];B[md];W[mh];B[mg];W[gf];B[me];W[mh];B[hd];W[he];B[mg]"
     ";W[jm];B[km];W[mh];B[ie];W[hc];B[mg];W[ga];B[lf];W[fa];B[eb];W[fg];B[hm];W[bj];B[ci];W[ai];B[ch];W[ff];B[jf];W["
     "jh];B[fm];W[em];B[kg];W[ea];B[da];W[ad];B[ab];W[ja];B[ka];W[ia];B[jm];W[hi];B[gh];W[gm];B[dg];W[eg];B[fm];W[hd];"
     "B[gm];W[gi];B[ei];W[di];)",
     "(;FF[4]GM[1]SZ[13]PB[bot1]PW[bot0]HA[0]KM[7]RU[koPOSITIONALscoreAREAtaxALLsui0button1];B[jd];W[jj];B[dd];W[dj];B["
     "ch];W[kf];B[fk];W[dh];B[dg];W[eh];B[ci];W[cj];B[hk];W[cg];B[cf];W[bg];B[bf];W[hc];B[ie];W[dc];B[eg];W[ed];B[jg];"
     "W[kh];B[cc];W[db];B[dl];W[gj];B[hj];W[fg];B[ff];W[gf];B[fe];W[gh];B[hg];W[ek];B[el];W[gk];B[gl];W[fj];B[fl];W[gg]"
     ";B[ge];W[hi];B[ii];W[ij];B[ji];W[ki];B[gc];W[cd];B[bh];W[hh];B[ig];W[bk];B[fi];W[il];B[hl];W[bl];B[bj];W[aj];B["
     "bi];W[ck];B[jl];W[bd];B[kk];W[kj];B[ei];W[di];B[ej];W[gi];B[dk];)",
     "(;FF[4]GM[1]SZ[13]PB[bot0]PW[bot1]HA[0]KM[7.5]RU[koSITUATIONALscoreAREAtaxNONEsui0];B[jd];W[kj];B[dc];W[cj];B[de]"
     ";W[kf];B[jj];W[jk];B[ki];W[kk];B[ji];W[hj];B[le];W[hg];B[ie];W[jg];B[dj];W[ci];B[ck];W[gk];B[gh];W[hh];B[dk];W["
     "be];B[bg];W[cd];B[dd];W[cc];B[bf];W[ce];B[cb];W[cf];B[cg];W[bb];B[db];W[df];B[dh];W[ic];B[jc];W[fe];B[gd];W[ke];"
     "B[gi];W[hi];B[ba];W[ge];B[hd];W[lc];B[gg];W[hf];B[ld];W[kd];B[kc];W[lb];B[ef];W[ee];B[eg];W[fc];B[fd];W[ed];B[ec]"
     ";W[fb];B[bc];W[hb];B[kb];W[id];B[la];W[mb];B[mc];W[md];B[me];W[je];B[mc];W[el];B[lg];W[lf];B[mf];W[md];B[ib];W["
     "hc];B[mc];W[ab];B[ac];W[md];B[mg];W[ia];B[mc];W[di];B[ma];W[bk];B[bl];W[ei];B[ek];W[fk];B[dl];W[fj];B[fl];W[gl];"
     "B[em];W[li];B[kg];W[jf];B[lj];W[lk];B[gm];W[hm];B[fm];W[hl];B[bj];W[bi];B[gf];W[he];B[ak];W[eh];B[dg];W[jb];B[jh]"
     ";W[eb];B[da];W[ea];)",
     "(;FF[4]GM[1]SZ[13]PB[bot1]PW[bot0]HA[0]KM[7]RU[koSITUATIONALscoreAREAtaxALLsui0];B[jj];W[dj];B[dd];W[jd];B[gk];W["
     "cg];B[hc];W[kh];B[dk];W[ck];B[ek];W[cc];B[kf];W[jf];B[cj];W[ci];B[bj];W[di];B[bk];W[dc];B[jg];W[kg];B[jh];W[lf];"
     "B[ke];W[je];B[le];W[ki];B[kj];W[ld];B[lg];W[kd];B[mf];W[fd];B[gf];W[gd];B[df];W[cl];B[bl];W[hg];B[ji];W[fg];B[ff]"
     ";W[gg];B[cd];W[ed];B[cf];W[fj];B[ec];W[eb];B[fc];W[gc];B[fb];W[gb];B[bc];W[bb];B[db];W[cb];B[ac];W[be];B[bd];W["
     "fa];B[dg];W[bg];B[bf];W[fi];B[fk];W[ae];B[bi];W[ch];B[ag];W[ah];B[af];W[ef];B[ee];W[eg];B[bh];)",
     "(;FF[4]GM[1]SZ[13]PB[bot1]PW[bot0]HA[0]KM[7.5]RU[koPOSITIONALscoreAREAtaxSEKIsui0button1];B[dd];W[jc];B[dj];W[jk]"
     ";B[ji];W[kf];B[gk];W[fd];B[jd];W[kd];B[ic];W[je];B[id];W[jb];B[kc];W[kb];B[hf];W[cc];B[cd];W[dc];B[ed];W[ec];B["
     "fe];W[gd];B[ie];W[ge];B[bc];W[bb];B[be];W[ac];B[lc];W[ld];B[jf];W[ke];B[jg];W[ff];B[lb];W[la];B[li];W[hl];B[gl];"
     "W[hk];B[lk];W[ll];B[kl];W[kk];B[lm];W[ml];B[mk];W[lj];B[mm];W[ki];B[kh];W[kj];B[lh];W[gj];B[ek];W[ih];B[gh];W[ii]"
     ";B[jh];W[fi];B[fh];W[ei];B[eg];W[eh];B[fg];W[ci];B[bj];W[cj];B[ck];W[dl];B[dk];W[bi];B[ij];W[jj];B[bk];W[cg];B["
     "cf];W[dg];B[ef];W[di];B[hi];W[hj];B[bg];W[jl];B[mj];W[hm];B[jm];W[fk];B[fl];W[el];B[fj];W[gi];B[ka];W[fm];B[ib];"
     "W[bh];B[gb];W[fb];B[bd];W[gc];B[hb];W[ab];B[hh];W[ee];B[cl];W[gm];B[fk];W[em];B[ag];W[ik];B[im];W[cm];B[fa];W[ea]"
     ";B[ga];W[ig];B[da];W[eb];B[ba];W[cb];B[bm];W[al];B[hg];W[ca];B[if];W[ij];)",
     "(;FF[4]GM[1]SZ[13]PB[bot0]PW[bot1]HA[0]KM[7]RU[koSITUATIONALscoreTERRITORYtaxALLsui1];B[dj];W[jj];B[hk];W[dd];B["
     "jd];W[kg];B[gc];W[cg];B[dc];W[ec];B[cc];W[fc];B[cd];W[gd];B[de];W[hc];B[ci];W[fg];B[ib];W[dh];B[ii];W[ej];B[ek];"
     "W[fk];B[el];W[fi];B[fl];W[be];B[ji];W[ld];B[ki];W[li];B[lj];W[lh];B[jf];W[jg];B[le];W[ke];B[kd];W[lc];B[kf];W[lf]"
     ";B[me];W[if];B[je];W[kb];B[ig];W[ih];B[hg];W[hh];B[hf];W[ie];B[id];W[he];B[hd];W[ge];B[gb];W[fb];B[hb];W[ed];B["
     "hi];W[gg];B[mf];W[lg];B[jb];W[bh];B[bd];W[ce];B[db];W[bb];B[ab];W[lk];B[kj];W[mj];B[ad];W[bj];B[bi];W[ck];B[dk];"
     "W[ai];B[kk];W[ga];B[ll];W[fa];B[kc];W[lb];B[ic];W[da];B[ca];W[ba];B[ea];W[cb];B[bc];W[da];B[ch];W[bg];B[ca];W[gk]"
     ";B[gl];W[da];B[gh];W[jh];B[ca];W[md];B[aa];W[mb];B[cl];)",
     "(;FF[4]GM[1]SZ[13]PB[bot1]PW[bot0]HA[0]KM[7]RU[koSIMPLEscoreTERRITORYtaxNONEsui0];B[jj];W[kd];B[dk];W[dd];B[cf];"
     "W[fc];B[ef];W[dj];B[ek];W[cj];B[ck];W[bk];B[ej];W[bi];B[di];W[ei];B[fi];W[eh];B[ch];W[cl];B[dl];W[cm];B[bh];W[al]"
     ";B[jd];W[ke];B[jc];W[fh];B[gi];W[kc];B[fe];W[bd];B[kg];W[gk];B[gh];W[je];B[gb];W[fb];B[gc];W[fd];B[gd];W[fj];B["
     "kb];W[lb];B[jb];W[jg];B[jh];W[he];B[la];W[ge];B[fg];W[ka];B[fa];W[ee];B[ff];W[id];B[cc];W[ea];B[cd];W[cb];B[ja];"
     "W[lc];B[dc];W[db];B[if];W[hb];B[dm];W[kk];B[jk];W[jl];B[il];W[kl];B[li];W[ik];B[hk];W[hl];B[ij];W[im];B[ik];W[fk]"
     ";B[eg];W[lj];B[gl];W[ml];B[bc];W[bb];B[ie];W[hd];B[lf];W[le];)",
     "(;FF[4]GM[1]SZ[13]PB[bot0]PW[bot1]HA[0]KM[7]RU[koSITUATIONALscoreAREAtaxALLsui1];B[jd];W[dd];B[fc];W[jj];B[dj];W["
     "ch];B[gk];W[kg];B[jk];W[ik];B[kk];W[hk];B[kj];W[gj];B[ji];W[fk];B[ke];W[hg];B[cf];W[df];B[hi];W[gi];B[bi];W[jh];"
     "B[ij];W[gd];B[gc];W[fd];B[hd];W[bh];B[dc];W[ec];B[eb];W[ed];B[cc];W[db];B[fb];W[cb];B[cd];W[bc];B[de];W[ee];B[ce]"
     ";W[dg];B[bb];W[ba];B[ab];W[ac];B[da];W[aa];B[bb];W[ab];B[ea];W[he];B[hh];W[gh];B[ig];W[if];B[ge];W[ff];B[ie];W["
     "hf];B[ca];W[kc];B[el];W[ib];B[hc];W[cl];B[gl];W[hj];B[bk];W[ek];B[dk];W[dl];B[fl];W[ei];B[bl];W[hl];B[cm];W[ae];"
     "B[jc];W[ld];B[le];W[jb];B[lb];W[lc];B[la];W[kb];B[hb];W[lh];B[ki];W[me];B[lf];W[md];B[mb];W[ia];B[ka];W[mf];B[lg]"
     ";W[kh];B[mg];W[mc];B[ma];W[li];B[lj];)",
     "(;FF[4]GM[1]SZ[13]PB[bot1]PW[bot0]HA[0]KM[7]RU[koPOSITIONALscoreAREAtaxALLsui1button1];B[dj];W[jd];B[jj];W[dd];B["
     "cf];W[kh];B[jh];W[jg];B[ih];W[dk];B[ck];W[ek];B[cj];W[il];B[kg];W[kk];B[jf];W[hc];B[kj];W[gk];B[jk];W[jl];B[kl];"
     "W[ce];B[df];W[fd];B[fj];W[ej];B[hk];W[lk];B[hl];W[ei];B[dh];W[ll];B[gj];W[ig];B[hg];W[if];B[kf];W[hi];B[hh];W[di]"
     ";B[ci];W[cl];B[bl];W[bm];B[bk];W[hj];B[ik];W[gl];B[im];W[el];B[jm];W[gh];B[fh];W[eh];B[ld];W[fg];B[bf];W[lc];B["
     "kc];W[kd];B[lb];W[le];B[mc];W[lh];B[ie];W[lj];B[gg];W[fi];B[gd];W[gc];B[fe];W[ec];B[bd];W[be];B[cd];W[de];B[ae];"
     "W[cc];B[bc];W[cb];B[bb];W[jc];B[jb];W[ba];B[ke];W[lm];B[ee];W[ed];B[ga];W[ad];B[ac];W[ef];B[ff];W[eg];B[dg];W[ab]"
     ";B[aa];W[he];B[hf];W[ab];B[fk];W[bg];B[aa];W[ag];B[af];W[ch];B[ca];W[bi];B[db];W[eb];B[dc];W[hd];B[gf];W[gb];B["
     "aj];W[bj];B[ai];W[bh];B[ak];)",
     "(;FF[4]GM[1]SZ[13]PB[bot1]PW[bot0]HA[0]KM[7.5]RU[koSIMPLEscoreTERRITORYtaxNONEsui0];B[jd];W[dc];B[kj];W[dk];B[ij]"
     ";W[hc];B[di];W[ci];B[dd];W[ec];B[cc];W[dh];B[cb];W[jb];B[kc];W[ie];B[je];W[if];B[jf];W[ce];B[cd];W[be];B[ge];W["
     "fd];B[fe];W[id];B[ed];W[fc];B[jc];W[ib];B[eh];W[ei];B[dg];W[ch];B[fi];W[ej];B[ig];W[hk];B[hj];W[gk];B[kb];W[db];"
     "B[ik];W[hg];B[hf];W[ih];B[jg];W[eg];B[fh];W[fg];B[ic];W[gi];B[ja];W[hb];B[gd];W[ea];B[fb];W[ia];B[gc];W[gb];B[hd]"
     ";W[ka];B[il];W[gh];B[gl];W[fl];B[hl];W[lb];B[el];W[fm];B[fk];W[ek];)",
     "(;FF[4]GM[1]SZ[13]PB[bot0]PW[bot1]HA[0]KM[7.5]RU[koSITUATIONALscoreAREAtaxNONEsui0];B[dj];W[jk];B[dd];W[jc];B[je]"
     ";W[kh];B[hc];W[fj];B[jj];W[kj];B[ik];W[ji];B[ij];W[jl];B[kk];W[kl];B[hh];W[ck];B[lk];W[ii];B[gj];W[hi];B[lj];W["
     "ki];B[ll];W[hj];B[hl];W[hk];B[il];W[gl];B[jm];W[dk];B[he];W[cf];B[fk];W[ej];B[kd];W[gi];B[ce];W[cc];B[df];W[fb];"
     "B[gb];W[kc];B[fc];W[ib];B[be];W[id];B[lc];W[lb];B[ie];W[eb];B[ld];W[gd];B[gc];W[mb];B[dc];W[hd];B[fd];W[dg];B[cg]"
     ";W[ch];B[bf];W[ge];B[lg];W[kf];B[lf];W[kg];B[lh];W[bh];B[eg];W[dh];B[ha];W[ia];B[jd];W[ic];B[km];W[hm];B[bg];W["
     "ef];B[ah];W[ee];B[gf];W[ai];B[ag];W[bi];B[fg];W[fe];B[de];W[hg];B[ig];W[gh];B[gg];W[ih];B[hf];W[eh];)",
     "(;FF[4]GM[1]SZ[13]PB[bot0]PW[bot1]HA[0]KM[7]RU[koPOSITIONALscoreTERRITORYtaxSEKIsui1];B[dd];W[jj];B[jc];W[cj];B["
     "kh];W[hk];B[je];W[cf];B[dj];W[ci];B[gk];W[hj];B[ck];W[bk];B[df];W[dg];B[ef];W[ce];B[cd];W[jb];B[kc];W[fc];B[hc];"
     "W[de];B[ed];W[ee];B[fd];W[fe];B[bl];W[cl];B[dk];W[al];B[hl];W[il];B[gl];W[gj];B[jl];W[im];B[fj];W[fi];B[fl];W[ej]"
     ";B[dl];W[bm];B[ek];W[ei];B[em];W[ki];B[li];W[lj];B[ik];W[gd];B[gc];W[bd];B[bc];W[cc];B[be];W[ad];B[ae];W[dc];B["
     "ac];W[bd];B[ad];W[ec];B[bd];W[hd];B[cb];W[ic];B[ib];W[id];B[hb];W[kb];B[lb];W[lc];B[ld];W[db];B[bb];W[la];B[mb];"
     "W[fb];B[ea];W[jk];B[ji];W[kj];B[if];W[hf];B[hg];W[jg];B[kg];W[ig];B[jf];W[hh];B[gg];W[ih];B[gf];W[ge];B[he];W[da]"
     ";B[ca];W[ia];B[ka];W[ja];B[ha];W[aa];B[bf];W[bg];B[ka];)"});
  return sgfs;
}

std::vector<string> TestCommon::getMultiGameSize19Data() {
  std::vector<string> sgfs(
    {"(;FF[4]GM[1]SZ[19]PB[bot1]PW[bot0]HA[0]KM[6.5]RU[koSITUATIONALscoreTERRITORYtaxNONEsui0];B[dq];W[pq];B[pd];W[cd];"
     "B[ec];W[co];B[ep];W[qn];B[df];W[cl];B[dd];W[ce];B[cf];W[qf];B[nc];W[de];B[ee];W[ef];B[be];W[fe];B[ed];W[cc];B[gc]"
     ";W[eg];B[ci];W[ch];B[bh];W[bg];B[dh];W[bf];B[cg];W[bd];B[db];W[cb];B[he];W[gg];B[jf];W[gi];B[fi];W[ei];B[eh];W["
     "fh];B[fj];W[dj];B[gj];W[hi];B[dk];W[cj];B[ej];W[bj];B[di];W[em];B[dn];W[hj];B[dm];W[dl];B[el];W[fm];B[bm];W[gk];"
     "B[bi];W[ek];B[ag];W[ae];B[fk];W[fl];B[hh];W[ek];B[ei];W[gh];B[el];W[cm];B[cn];W[ek];B[hk];W[gl];B[el];W[bl];B[bo]"
     ";W[ek];B[pp];W[ck];B[qq];W[qp];B[oq];W[pr];B[qr];W[or];B[qo];W[rp];B[ro];W[rq];B[rr];W[po];B[op];W[rn];B[oo];W["
     "pn];B[nr];W[md];B[pf];W[mc];B[kc];W[kd];)",
     "(;FF[4]GM[1]SZ[19]PB[bot0]PW[bot1]HA[0]KM[7.5]RU[koSIMPLEscoreAREAtaxNONEsui0button1];B[pd];W[dd];B[fc];W[cp];B["
     "pp];W[cf];B[eq];W[dn];B[qf];W[nq];B[qn];W[gq];B[lq];W[jq];B[lo];W[jo];B[lm];W[pg];B[pj];W[nc];B[lc];W[ne];B[of];"
     "W[pf];B[pe];W[qg];B[rf];W[kd];B[og];W[kc];B[mf];W[ec];B[fd];W[oh];B[nh];W[oi];B[ld];W[le];B[me];W[md];B[mb];W[ke]"
     ";B[nb];W[oc];B[ob];W[pc];B[qc];W[nf];B[mg];W[qb];B[mc];W[qd];B[nd];W[qe];B[od];W[rc];B[rg];W[qh];B[rh];W[qi];B["
     "re];W[rd];B[ri];W[qj];B[rk];W[qk];B[rl];W[pb];B[nk];W[gf];B[mj];W[ok];B[ol];W[pl];B[oj];W[om];B[pk];W[ql];B[qm];"
     "W[ml];B[kk];W[nl];B[ok];W[mk];B[ni];W[kl];B[ll];W[no];B[nn];W[lk];B[nm];W[jk];B[kj];W[km];B[ln];W[jj];B[ef];W[ee]"
     ";B[ed];W[de];B[dc];W[cc];B[eb];W[cb];B[cl];W[cj];B[dj];W[ci];B[id];W[hc];B[hd];W[ki];B[lj];W[jf];B[ic];W[hg];B["
     "el];W[dm];B[dl];W[fk];B[fn];W[fl];B[em];W[eo];B[hn];W[fo];B[go];W[gp];B[jn];W[io];B[cq];W[dp];B[bp];W[bo];B[bq];"
     "W[dq];B[dr];W[er];B[hl];W[gm];B[fj];W[gj];B[gi];W[ej];B[fi];W[ek];B[hj];W[gk];B[hk];W[kn];B[jh];W[ii];B[ih];W[hh]"
     ";B[hi];W[jl];B[dk];W[ei];B[di];W[eh];B[dh];W[eg];B[bg];W[bk];B[bl];W[ch];B[cg];W[dg];B[bh];W[bi];B[aj];W[ah];B["
     "ck];W[ag];B[bj];W[bf];B[bn];W[ao];B[an];W[cn];B[cm];W[cr];B[oq];W[lr];B[mr];W[mq];B[kq];W[pr];B[or];W[qq];B[os];"
     "W[qp];B[ro];W[nr];B[op];W[kr];B[gn];W[oo];B[po];W[hm];B[np];W[jb];B[ib];W[ms];B[kg];W[ji];B[ns];W[db];B[mp];W[mr]"
     ";B[jr];W[kp];B[lp];W[ir];B[in];W[ia];B[ha];W[ko];B[if];W[ig];B[hf];W[jg];B[ge];W[kh];B[ja];W[ka];B[ff];W[gg];B["
     "fe];W[ia];B[je];W[lg];B[ja];W[li];B[im];W[fm];B[en];W[mi];B[il];W[nj];B[mj];W[ec];B[fa];W[gb];B[ea];W[ie];B[he];"
     "W[jd];B[gc];W[lj];B[nj];W[mh];B[lb];W[ia];B[kb];W[jc];B[ja];W[mm];B[mn];W[ia];B[ho];W[hp];B[ja];W[rj];B[sj];W[ia]"
     ";B[lf];W[kf];B[ja];W[ng];B[oe];W[ia];B[rb];W[da];B[dc];W[rr];B[rp];W[ec];B[ja];W[fb];B[ga];W[ia];B[dc];W[ja];B["
     "oa];W[ec];B[hb];W[ra];B[la];W[se];B[sf];W[pa];B[sd];W[sc];B[se];W[fg];B[df];W[gl];B[ak];W[ai];B[jm];W[ik];B[ij];"
     "W[gh];B[fh];)",
     "(;FF[4]GM[1]SZ[19]PB[bot1]PW[bot0]HA[0]KM[7]RU[koSIMPLEscoreTERRITORYtaxSEKIsui1];B[cd];W[dp];B[qd];W[qp];B[fd];"
     "W[oc];B[pe];W[lc];B[oq];W[po];B[cn];W[fp];B[kp];W[qi];B[eo];W[ep];B[jc];W[qb];B[md];W[mc];B[nd];W[dd];B[dc];W[ld]"
     ";B[le];W[ke];B[lf];W[nc];B[kf];W[jd];B[bp];W[iq];B[ho];W[hp];B[qk];W[qf];B[og];W[ph];B[qn];W[oh];B[om];W[no];B["
     "je];W[kd];B[rq];W[pn];B[pm];W[qq];B[rp];W[ro];B[qo];W[rn];B[qm];W[rr];B[pp];W[pq];B[op];W[nn];B[lq];W[ml];B[nk];"
     "W[lo];B[rh];W[ri];B[rf];W[qg];B[re];W[jn];B[jo];W[ko];B[io];W[jp];B[mk];W[rg];B[gp];W[jq];B[gq];W[go];B[im];W[gr]"
     ";B[gm];W[fo];B[kl];W[mq];B[qr];W[pr];B[mr];W[or];B[rs];W[qs];B[sr];W[ss];B[nh];W[ni];B[rs];W[nr];B[qr];W[mi];B["
     "mh];W[li];B[ji];W[rr];B[oj];W[oi];B[qj];W[kj];B[ik];W[jh];B[ii];W[mf];B[lg];W[of];B[pf];W[ih];B[hh];W[hg];B[kh];"
     "W[ki];B[ig];W[ng];B[mg];W[pg];B[oe];W[jg];B[jf];W[if];B[kg];W[rj];B[rk];W[lk];B[ll];W[kk];B[qr];W[jl];B[jm];W[km]"
     ";B[ps];W[ig];B[ie];W[hi];B[ij];W[lr];B[lm];W[id];B[he];W[sh];B[dh];W[di];B[ch];W[ei];B[eh];W[cc];B[sj];W[mj];B["
     "de];W[cm];B[dm];W[cl];B[bn];W[dn];B[em];W[ci];B[do];W[bh];B[bg];W[fi];B[bi];W[bj];B[ah];W[gh];B[gj];W[fn];B[en];"
     "W[dk];B[fl];W[fk];B[gk];W[el];B[dl];W[ek];B[fm];W[al];B[aj];W[gf];B[bk];W[cj];B[fg];W[fh];B[hd];W[ak];B[bl];W[ai]"
     ";B[fq];W[fr];B[aj];W[eg];B[ff];W[ai];B[eq];W[dq];B[aj];W[gg];B[gi];W[ai];B[er];W[hq];B[aj];W[fe];B[ef];W[ai];B["
     "nq];W[ms];B[aj];W[ge];B[ed];W[ai];B[mp];W[bm];B[mr];W[nf];B[mq];W[hc];B[gd];W[me];B[aj];W[ck];B[hb];W[nl];B[ol];"
     "W[ln];B[kn];W[ic];B[km];W[kq];B[lp];W[gb];B[fb];W[gc];B[ga];W[ns];B[kr];W[ks];B[ls];W[ok];B[pk];W[lr];B[jr];W[ir]"
     ";B[ls];W[nj];B[ok];W[lr];B[sg];W[si];B[ls];W[hn];B[gn];W[lr];B[fj];W[ej];B[ls];W[ip];B[in];W[lr];B[pi];W[sk];B["
     "ls];)",
     "(;FF[4]GM[1]SZ[19]PB[bot0]PW[bot1]HA[0]KM[6.5]RU[koSITUATIONALscoreTERRITORYtaxSEKIsui0];B[dc];W[pp];B[dq];W[qd];"
     "B[qq];W[pq];B[qp];W[qn];B[rn];W[dp];B[cp];W[eq];B[cq];W[rm];B[qo];W[pn];B[po];W[oo];B[op];W[or];B[np];W[no];B[mo]"
     ";W[on];B[qr];W[mp];B[nq];W[mq];B[nr];W[mr];B[ns];W[ep];B[mn];W[cn];B[jp];W[co];B[er];W[fr];B[ds];W[jq];B[iq];W["
     "ip];B[hq];W[jr];B[io];W[ms];B[pr];W[kp];B[hp];W[ko];B[fq];W[ck];B[oc];W[mc];B[pj];W[nk];B[nj];W[mj];B[mk];W[lj];"
     "B[ml];W[ok];B[oj];W[pk];B[qk];W[ql];B[rk];W[ro];B[qg];W[kk];B[pe];W[qe];B[pf];W[qb];B[km];W[fp];B[gq];W[de];B[ce]"
     ";W[cf];B[cd];W[df];B[fc];W[qi];B[qj];W[rg];B[rh];W[rf];B[il];W[ri];B[qh];W[mh];B[me];W[sh];B[pi];W[nd];B[mg];W["
     "lg];B[pc];W[mf];B[ne];W[le];B[ld];W[ke];B[lc];W[ed];B[fd];W[fe];B[hk];W[ec];B[eb];W[db];B[cb];W[fb];B[da];W[gm];"
     "B[fk];W[jl];B[jm];W[im];B[jn];W[in];B[jo];W[gj];B[gk];W[ik];B[hl];W[gn];B[ij];W[jk];B[dk];W[dj];B[ej];W[dl];B[el]"
     ";W[dm];B[di];W[cj];B[gh];W[eh];B[ge];W[gd];B[gc];W[gf];B[hd];W[hg];B[ih];W[hh];B[hi];W[ji];B[jj];W[ki];B[ii];W["
     "if];B[jf];W[je];B[ie];W[hf];B[kh];W[jg];B[jh];W[lh];B[em];W[go];B[ei];W[bh];B[ch];W[dh];B[ci];W[bi];B[bg];W[bf];"
     "B[ff];W[fg];B[ee];W[ef];B[dd];W[gi];B[be];W[jc];B[af];W[cg];B[ah];W[ai];B[eg];W[ag];B[fh];W[dg];B[fi];W[gg];B[ir]"
     ";W[lo];B[kr];W[kq];B[js];W[ks];)",
     "(;FF[4]GM[1]SZ[19]PB[bot1]PW[bot0]HA[0]KM[7]RU[koSIMPLEscoreAREAtaxNONEsui0];B[pp];W[dp];B[pd];W[dc];B[cq];W[cp];"
     "B[dq];W[ep];B[fq];W[fp];B[gq];W[eq];B[er];W[fr];B[dr];W[hq];B[gr];W[gp];B[hr];W[ci];B[iq];W[ei];B[dd];W[ec];B[cd]"
     ";W[cc];B[bc];W[bb];B[be];W[ac];B[qn];W[qc];B[qd];W[pc];B[nc];W[oc];B[od];W[nb];B[mc];W[mb];B[lc];W[pj];B[ad];W["
     "bd];B[rc];W[rb];B[bc];W[pq];B[ab];W[qq];B[qb];W[pb];B[np];W[nq];B[gc];W[ed];B[ra];W[qa];B[sb];W[sa];B[ge];W[ic];"
     "B[ee];W[gb];B[fb];W[gd];B[fd];W[fc];B[hd];W[hc];B[gd];W[eb];B[ra];W[kc];B[dj];W[di];B[bn];W[bo];B[dn];W[em];B[en]"
     ";W[fn];B[fm];W[el];B[gn];W[fo];B[gm];W[cm];B[fk];W[ck];B[kd];W[jd];B[kb];W[jc];B[lb];W[mp];B[op];W[oq];B[je];W["
     "ke];B[ld];W[fa];B[qb];W[ql];B[qp];W[rb];B[rq];W[rr];B[qb];W[ie];B[pa];W[mn];B[hp];W[jf];B[cn];W[bm];B[aq];W[bp];"
     "B[ro];W[sq];B[pl];W[pm];B[rl];W[qk];B[rk];W[qm];B[rm];W[ho];B[im];W[jo];B[km];W[rj];B[qi];W[qj];B[ri];W[si];B[ll]"
     ";W[hj];B[gj];W[hk];B[gk];W[hi];B[fh];W[gh];B[fi];W[fg];B[eh];W[cg];B[gg];W[hg];B[dh];W[gf];B[am];W[ch];B[go];W["
     "ip];B[hq];W[df];B[de];W[bq];B[br];W[kp];B[ak];W[bj];B[ef];W[ar];B[as];W[bl];B[al];W[ej];B[cf];W[ap];B[ar];W[an];"
     "B[ao];W[cr];B[cs];W[an];B[eo];W[pn];B[nn];W[nm];B[mo];W[lo];B[no];W[sn];B[sm];W[po];B[sk];W[qo];B[rn];W[rp];B[sj]"
     ";W[bk];B[ao];W[aj];B[an];W[dm];B[oh];W[ng];B[kr];W[og];B[nh];W[pg];B[ph];W[mf];B[mg];W[mh];B[lg];W[lh];B[kg];W["
     "kh];B[nf];W[lf];B[lq];W[mm];B[qg];W[ne];B[pf];W[of];B[kj];W[mj];)",
     "(;FF[4]GM[1]SZ[19]PB[bot0]PW[bot1]HA[0]KM[7]RU[koSIMPLEscoreTERRITORYtaxSEKIsui1];B[dq];W[cd];B[ec];W[pd];B[pp];"
     "W[co];B[ep];W[de];B[qf];W[nd];B[oe];W[od];B[rd];W[hc];B[cl];W[qn];B[nq];W[dn];B[ci];W[bq];B[qo];W[pn];B[ql];W[qj]"
     ";B[ol];W[nn];B[oj];W[rn];B[qk];W[oh];B[qh];W[lo];B[lq];W[op];B[pq];W[oq];B[or];W[np];B[mr];W[fo];B[gp];W[qe];B["
     "re];W[pf];B[pg];W[og];B[pe];W[of];B[qd];W[ne];B[ee];W[ch];B[bh];W[dh];B[bg];W[ef];B[fe];W[he];B[dd];W[dc];B[ed];"
     "W[cf];B[cc];W[bc];B[db];W[cb];B[ff];W[hg];B[gh];W[ei];B[dk];W[fj];B[gg];W[hh];B[gi];W[ij];B[hi];W[ii];B[hk];W[dc]"
     ";B[gk];W[cj];B[bj];W[be];B[lm];W[po];B[rp];W[ro];B[ki];W[jh];B[rq];W[di];B[bi];W[bm];B[cm];W[bl];B[bk];W[cn];B["
     "dj];W[jo];B[cc];W[ba];B[br];W[cq];B[cr];W[eo];B[go];W[dp];B[fp];W[mm];B[ml];W[ll];B[kl];W[lk];B[kn];W[ko];B[nm];"
     "W[mn];B[kk];W[lj];B[kj];W[jm];B[km];W[li];B[lh];W[mi];B[kh];W[ik];B[jn];W[in];B[kf];W[im];B[ld];W[jc];B[jd];W[kc]"
     ";B[kd];W[lc];B[id];W[hd];B[if];W[hf];B[jg];W[ie];B[mh];W[ni];B[je];W[ig];B[md];W[mc];B[jf];W[rj];B[pi];W[sk];B["
     "pb];W[fk];B[gb];W[hb];B[eg];W[df];B[fb];W[gc];B[gm];W[gn];B[fl];W[el];B[fn];W[hn];B[en];W[em];B[fm];W[do];B[dm];"
     "W[dr];B[eq];W[ek];B[fi];W[ej];B[eh];W[ga];B[ea];W[ha];B[fc];W[fa];B[dc];W[ar];B[er];W[pc];B[qb];W[lg];B[mg];W[le]"
     ";B[me];W[da];B[ca];W[gj];B[bb];W[hj];B[jq];W[iq];B[ip];W[jp];B[jr];W[lf];B[mf];W[rg];B[qg];W[hp];B[hq];W[io];B["
     "ir];W[qc];B[rc];W[ob];B[rh];W[ab];B[aa];W[qp];B[qq];W[ba];B[cb];W[nk];B[ok];W[nl];B[om];W[mp];B[nj];W[mk];B[qe];"
     "W[mq];B[nr];W[pa];B[rb];W[nf];B[si];W[sj];B[rl];W[sl];B[ic];W[ib];B[kq];W[al];B[ak];W[bn];B[qa];W[oa];B[jl];W[bs]"
     ";B[ds];W[ri];B[sh];W[af];B[il];W[hl];B[on];W[oo];B[bf];W[ag];B[ad];W[ac];B[ae];W[bd];B[ae];W[ad];B[ah];W[ae];B["
     "ip];W[ho];B[sp];W[gf];B[pj];W[pm];B[pl];W[ji];B[aa];W[ce];B[ba];W[as];B[iq];W[qo];B[so];W[sn];B[ck];W[gl];B[fg];"
     "W[sm];B[ph];W[aq];B[cs];W[mj];B[ng];W[ge];B[fd];W[gd];B[ln];W[lp];B[cg];W[dg];B[nh];W[oi];B[kp];W[rk];B[qi];W[jj]"
     ";B[qm];W[dl];B[rm];W[jk];B[];W[];B[];W[];B[rf];W[hm];B[sg];W[nc];B[es];W[ih];B[lr];W[fn];B[pr];W[cp];B[gq];W[nb];"
     "B[];W[])",
     "(;FF[4]GM[1]SZ[19]PB[bot0]PW[bot1]HA[0]KM[6.5]RU[koSITUATIONALscoreTERRITORYtaxSEKIsui0];B[dq];W[dd];B[qp];W[qd];"
     "B[dn];W[oq];B[pn];W[kp];B[oc];W[pe];B[cf];W[fd];B[lc];W[ce];B[qh];W[cl];B[iq];W[dp];B[cp];W[co];B[do];W[ep];B[cq]"
     ";W[cn];B[dm];W[dl];B[fn];W[fl];B[lq];W[lp];B[mp];W[mo];B[np];W[kq];B[mr];W[lr];B[mq];W[gp];B[cm];W[bm];B[bl];W["
     "bk];B[bn];W[al];B[bo];W[qq];B[rp];W[ip];B[no];W[pp];B[rq];W[jc];B[mn];W[md];B[mc];W[qj];B[pi];W[ql];B[ni];W[pc];"
     "B[od];W[oe];B[ne];W[nd];B[pb];W[nf];B[qb];W[ol];B[pj];W[pk];B[nk];W[qi];B[pg];W[oh];B[og];W[ld];B[mg];W[kd];B[ee]"
     ";W[ed];B[be];W[bd];B[eh];W[rh];B[rg];W[ph];B[qg];W[nh];B[ng];W[mh];B[lf];W[nc];B[nb];W[mf];B[lh];W[mi];B[nj];W["
     "li];B[kh];W[ki];B[ji];W[jj];B[nl];W[kk];B[ii];W[po];B[qn];W[on];B[nm];W[om];B[oo];W[rr];B[rn];W[rm];B[pr];W[qr];"
     "B[or];W[pq];B[sr];W[sn];B[he];W[if];B[hf];W[jg];B[jh];W[hg];B[gg];W[hh];B[gh];W[hi];B[ij];W[ik];B[hk];W[hj];B[il]"
     ";W[jk];B[gk];W[km];B[je];W[ie];B[id];W[jd];B[re];W[hd];B[gj];W[rd];B[ri];W[rj];B[sh];W[sj];B[si];W[kb];B[rk];W["
     "sk];B[pm];W[qo];B[ro];W[sq];B[sp];W[nn];B[pl];W[ok];B[qk];W[rl];B[qk];W[rb];B[fe];W[gd];B[ig];W[ih];B[gi];W[ig];"
     "B[de];)",
     "(;FF[4]GM[1]SZ[19]PB[bot0]PW[bot1]HA[0]KM[7.5]RU[koSITUATIONALscoreAREAtaxNONEsui1];B[pq];W[pd];B[cp];W[cd];B[pn]"
     ";W[ep];B[hq];W[dn];B[eo];W[do];B[dp];W[eq];B[en];W[bo];B[bp];W[dm];B[em];W[dl];B[dq];W[gp];B[hp];W[go];B[gq];W["
     "el];B[fm];W[hm];B[gm];W[hl];B[fp];W[ed];B[dh];W[gl];B[hn];W[er];B[cf];W[ci];B[ch];W[bi];B[gg];W[eg];B[di];W[fl];"
     "B[ho];W[cr];B[dr];W[ds];B[br];W[bs];B[bq];W[fq];B[fo];W[gr];B[fs];W[es];B[hr];W[gs];B[co];W[bn];B[hs];W[fr];B[cq]"
     ";W[cn];B[cs];W[pp];B[fs];W[qq];B[oq];W[op];B[qr];W[qp];B[ck];W[bk];B[rr];W[np];B[nq];W[mq];B[mp];W[nr];B[os];W["
     "fg];B[gh];W[fi];B[fh];W[eh];B[ei];W[gf];B[hf];W[fe];B[bl];W[cj];B[cl];W[dk];B[dj];W[bh];B[bg];W[bm];B[dc];W[cc];"
     "B[dd];W[de];B[ec];W[fc];B[fb];W[gc];B[cb];W[bb];B[ce];W[db];B[eb];W[ca];B[ge];W[ff];B[ee];W[ef];B[df];W[gb];B[ic]"
     ";W[dg];B[cg];W[fd];B[ee];W[ib];B[jb];W[de];B[ak];W[bj];B[ee];W[ga];B[ig];W[gi];B[de];W[jc];B[jd];W[kc];B[hb];W["
     "id];B[ia];W[hd];B[kd];W[ie];B[jf];W[lc];B[fa];W[hc];B[da];W[ea];B[hi];W[hh];B[hg];W[hj];B[da];W[or];B[pr];W[ea];"
     "B[ah];W[cm];)",
     "(;FF[4]GM[1]SZ[19]PB[bot1]PW[bot0]HA[0]KM[6.5]RU[koSITUATIONALscoreTERRITORYtaxSEKIsui0];B[dq];W[dd];B[qp];W[qd];"
     "B[do];W[oq];B[pn];W[dn];B[cn];W[cm];B[dm];W[en];B[em];W[co];B[bn];W[fn];B[bo];W[bm];B[cp];W[fm];B[ek];W[cj];B[fl]"
     ";W[gl];B[gk];W[hl];B[hk];W[il];B[di];W[ci];B[dh];W[cg];B[ik];W[jl];B[jk];W[kl];B[dc];W[ec];B[ed];W[fc];B[cf];W["
     "bg];B[cc];W[ic];B[dg];W[bf];B[de];W[fd];B[be];W[dk];B[oc];W[lc];B[kq];W[iq];B[mq];W[oo];B[on];W[no];B[jo];W[gp];"
     "B[ip];W[jq];B[kp];W[hp];B[mn];W[qr];B[ml];W[rq];B[el];W[ej];B[dj];W[bk];B[or];W[nr];B[nq];W[pr];B[op];W[os];B[pq]"
     ";W[pe];B[ee];W[nd];B[qc];W[rc];B[rb];W[qb];B[pc];W[sb];B[oe];W[re];B[od];W[er];B[og];W[or];B[fq];W[fr];B[eq];W["
     "gq];B[ho];W[fp];B[cl];W[bl];B[hn];W[kk];B[kj];W[lj];B[lk];W[li];B[ki];W[lh];B[ld];W[kh];B[ii];W[mc];B[kd];W[kc];"
     "B[id];W[ih];B[hc];W[jd];B[je];W[jc];B[hd];W[fj];B[fk];W[hi];B[gj];W[oi];B[lf];W[hb];B[gb];W[ie];B[if];W[ke];B[he]"
     ";W[gh];B[hg];W[mf];B[kf];W[ji];B[jj];W[ij];B[gi];W[mg];B[hr];W[gr];B[dr];W[hm];B[pi];W[ph];B[oh];W[oj];B[mj];W["
     "mi];B[pj];W[pk];B[ok];W[nk];B[ol];W[mk];B[ll];W[jn];B[qh];W[pg];B[ib];W[jb];B[ha];W[qg];B[me];W[ne];B[nf];W[nb];"
     "B[qk];W[nl];B[nm];W[nj];B[ri];W[ln];B[lo];W[po];B[qo];W[pp];B[kn];W[lm];B[mm];W[km];B[in];W[mo];B[jm];W[ir];B[mp]"
     ";W[fh];B[fi];W[hh];B[kr];W[hs];B[qq];W[rp];B[ro];W[np];B[rg];W[rf];B[af];W[sg];B[ag];W[pl];B[ql];W[om];B[qm];W["
     "nn];B[jp];W[ai];B[am];W[ch];B[al];W[ck];B[dl];W[ah];B[bi];W[bd];B[ce];W[fe];B[ff];W[ef];B[eg];W[bc];B[bb];W[ab];"
     "B[cb];W[gf];B[fg];W[gg];B[ge];W[rh];B[qi];W[jg];B[jf];W[so];B[sn];W[sq];B[qn];W[sm];B[sp];W[im];B[jn];W[so];B[sl]"
     ";W[si];B[rj];W[sk];B[sj];W[rl];B[rk];W[rn];B[rm];W[md];B[le];W[ds];B[cs];W[es];B[cr];W[ja];B[mr];W[rr];B[sl];W["
     "ep];B[dp];W[js];B[jr];W[ks];B[ls];W[ia];B[hb];W[hj];B[ei];W[is];B[sh];W[ao];B[ap];W[si];B[ig];W[kg];B[gn];W[ms];"
     "B[lr];W[ns];B[sh];W[ak];B[bj];W[si];B[sn];W[sh];B[sp];)",
     "(;FF[4]GM[1]SZ[19]PB[bot0]PW[bot1]HA[0]KM[7]RU[koPOSITIONALscoreAREAtaxNONEsui1button1];B[dd];W[dp];B[pd];W[pp];"
     "B[qn];W[cc];B[dc];W[cd];B[cf];W[ce];B[de];W[bf];B[qf];W[nq];B[kd];W[cg];B[df];W[cb];B[fq];W[fp];B[gp];W[fo];B[er]"
     ";W[dg];B[go];W[fg];B[fn];W[dn];B[hd];W[cq];B[en];W[dm];B[qp];W[qq];B[rq];W[pq];B[ro];W[eq];B[gr];W[kq];B[ni];W["
     "pg];B[ng];W[qg];B[pf];W[qk];B[rf];W[ob];B[nc];W[qc];B[pc];W[pb];B[rc];W[oc];B[nd];W[qd];B[rd];W[od];B[pe];W[qi];"
     "B[ne];W[gc];B[gd];W[hq];B[gq];W[dr];B[db];W[gn];B[jq];W[jr];B[ir];W[kr];B[gk];W[hn];B[jp];W[jn];B[kp];W[hj];B[gj]"
     ";W[hi];B[hk];W[ik];B[il];W[jk];B[el];W[hc];B[id];W[jl];B[dl];W[fd];B[fe];W[kb];B[ic];W[ib];B[hb];W[gb];B[jb];W["
     "ha];B[ja];W[fl];B[em];W[bm];B[gm];W[hl];B[gl];W[io];B[hm];W[im];B[bl];W[iq];B[ip];W[hr];B[kn];W[lo];B[ko];W[km];"
     "B[ln];W[mm];B[mq];W[np];B[mo];W[is];B[hp];W[on];B[mp];W[gs];B[fs];W[hs];B[mr];W[ls];B[lm];W[ll];B[ml];W[nm];B[lk]"
     ";W[kl];B[om];W[nl];B[nn];W[pn];B[mk];W[nk];B[ji];W[ii];B[nj];W[pm];B[gh];W[jh];B[ki];W[gi];B[fi];W[fh];B[hg];W["
     "ih];B[ig];W[fj];B[ei];W[kh];B[li];W[ej];B[di];W[dj];B[ci];W[cj];B[bj];W[bi];B[ck];W[aj];B[bk];W[gg];B[bh];W[hh];"
     "B[bg];W[jf];B[if];W[kf];B[lg];W[lf];B[mf];W[eg];B[gf];W[hl];B[nr];W[or];B[il];W[lp];B[lq];W[hl];B[js];W[ks];B[il]"
     ";W[ie];B[hl];W[je];B[jd];W[jm];B[ff];W[no];B[le];)"});
  return sgfs;
}