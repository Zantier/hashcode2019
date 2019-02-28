" set exrc
nnoremap ,m :wa<cr>:!make && echo -n 'input filename (without .in): ' && read test && ./sol "$test"<cr>
nnoremap ,1 :wa<cr>:!make && ./sol a_example<cr>
nnoremap ,2 :wa<cr>:!make && ./sol b_lovely_landscapes<cr>
nnoremap ,3 :wa<cr>:!make && ./sol c_memorable_moments<cr>
nnoremap ,4 :wa<cr>:!make && ./sol d_pet_pictures<cr>
nnoremap ,5 :wa<cr>:!make && ./sol e_shiny_selfies<cr>
