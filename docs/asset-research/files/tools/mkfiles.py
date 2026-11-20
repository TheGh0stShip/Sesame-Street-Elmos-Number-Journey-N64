"""Write the coordinator-format FILES_<game>.tsv (id,start,end,size,class,note,loaded_by) from extract_files tables."""
import sys,importlib.util
spec=importlib.util.spec_from_file_location('ef',sys.argv[1]); ef=importlib.util.module_from_spec(spec); spec.loader.exec_module(ef)
CL={'gzip':'gzip_level','libmus_ptrbank':'libmus_ptr_bank','libmus_wavebank':'libmus_wave_bank','libmus_fxbank':'libmus_fx_bank','libmus_song':'libmus_song','mort_clips':'speech_mort','sprite_anim':'sprite_anim','raw_screen_images':'gfx_pack','raw_prelinked':'gfx_pack'}
rom=open(sys.argv[2],'rb').read(); g=ef.GAMES_BY(rom)
print('id\trom_start\trom_end\tsize\tclass_guess\tnote\tloaded_by')
for i,(a,b,c,f,n,d,r) in enumerate(g['ranges']):
    print('%d\t0x%06X\t0x%06X\t%d\t%s\t%s: %s\t%s'%(i,a,b,b-a,CL[f],n,d,r))
