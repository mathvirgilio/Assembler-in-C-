# tarefas + urgentes:

> 15/01

no montador:
- [x] implementar criação do arquivo `.obj`
- [x] implementar criação da linha "R":
	- lógica: começa com "R, "
	- em seguida, deve haver 0's ou 1's para indicar se uma posição do arquivo `.obj` em questão é ou não uma posição relativa, i.e. se deve ou não ser acrescida do ajuste
	- bit sinalizando `1` indica que a posição equivalente no arquivo objeto é uma posição relativa
- [ ] implementar mudanças no ligador para que ele reconheça o mapa de bits "R"

- [x] testar a conversão dos números(decimais) negativos para objetos do tipo `std::string`
- [ ] implementar conversão de `std::string` - único argumento da diretiva `CONST` - que contenha um número hexadecimal para outra que contenha o mesmo valor em decimal
	- **OBS.:** atentar-se para a representação de números hexadecimais negativos
	- **OBS.:** `0xF` deve ser convertido para `-1`


- [x] verificar em que momento do código os caracteres são passados para maiúsculo
	- [ ] talvez: realizar alguns ajustes
