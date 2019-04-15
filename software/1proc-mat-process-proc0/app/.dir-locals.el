 ;; ((nil . (eval . (setq flycheck-clang-include-path
 ;;                       (list (expand-file-name "../../common/")
 ;;                            (expand-file-name "../bsp/drivers/inc")
 ;;                            (expand-file-name "../bsp/")
 ;;                            (expand-file-name "../bsp/HAL/inc"))))))

((c-mode . (( flycheck-clang-include-path . (eval . (list
                                            (expand-file-name "../../common/")
                                            (expand-file-name "../bsp/drivers/inc")
                                            (expand-file-name "../bsp/")
                                            (expand-file-name "../bsp/HAL/inc")))))))
