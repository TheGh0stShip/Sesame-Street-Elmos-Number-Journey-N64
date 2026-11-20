import sys,collections
for g in ('letter','number'):
    c=collections.Counter(); n=collections.Counter()
    for l in open('${N64DK_LAB}/sesassets/ledger/LEDGER_%s.tsv'%g):
        x=l.rstrip('\n').split('\t')
        if x[0]=='start': continue
        k=(x[5], x[6].split(' ')[0] if x[5]!='code' else 'code', x[7]); s=int(x[1],16)-int(x[0],16); c[k]+=s; n[k]+=1
    print('==',g)
    for k,v in c.most_common(): print(v,n[k],*k,sep='\t')
