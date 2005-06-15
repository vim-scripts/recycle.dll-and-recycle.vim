" recycle.vim
" autocommands to facilitate recycling function in backuping files
" made by kAtremer <katremer@yandex.ru>
" just drop it in vimfiles/plugin along with recycle.dll

" TODO: for now, it does not recycle if 'patchmode' is set, keeps file instead
" TODO: pass &patchmode to recycle.dll when storing .orig
" TODO: pass &backupext to recycle.dll when backuping
" TODO: recycle on FileWritePre???
" TODO: copy the file with &backupext (like now)
"       or delete the original (like it was)???

if exists('loaded_recycle') || !&backup || &patchmode
	finish
endif
let loaded_recycle=1

set nobackup

function! Recycle()
	if !exists('b:norecycle')
		if !libcallnr(expand('<sfile>:p:h').'\Recycle', 'Recycle', expand('<afile>:p'))
			echohl Error
			echomsg 'Recycling '.expand('<afile>').' failed; enabling normal backup'
			echohl None
			set backup
		else
			set nobackup
		endif
	endif
endfunction

augroup recycle
	autocmd!
	execute 'autocmd BufWritePre '.&backupskip.' let b:norecycle=1'
	autocmd BufWritePre * call Recycle()
	"execute 'autocmd FileWritePre '.&backupskip.' let b:norecycle=1'
	"autocmd FileWritePre * call Recycle()
augroup END
