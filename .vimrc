" set exrc
nnoremap ,m :wa<cr>:!make && echo -n 'input filename (without .in): ' && read test && ./sol "$test"<cr>
nnoremap ,1 :wa<cr>:!make && ./sol test1<cr>
nnoremap ,2 :wa<cr>:!make && ./sol test2<cr>
nnoremap ,3 :wa<cr>:!make && ./sol test3<cr>
nnoremap ,4 :wa<cr>:!make && ./sol test4<cr>
nnoremap ,5 :wa<cr>:!make && ./sol test5<cr>
