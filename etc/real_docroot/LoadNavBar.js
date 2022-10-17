var nav = document.createElement('nav');
nav.className = "navbar fixed-top navbar-expand-lg navbar-dark bg-dark";
nav.innerHTML += " <a class='navbar-brand'			style='color: #fff;'>电子教学</a>		<button class='navbar-toggler' type='button' data-toggle='collapse' data-target='#navbarSupportedContent1'			aria-controls='navbarSupportedContent1' aria-expanded='false' aria-label='Toggle navigation'> <span				class='navbar-toggler-icon'></span> </button>		<div class='collapse navbar-collapse' id='navbarSupportedContent1'>			<ul class='navbar-nav mr-auto'>				<li class='nav-item'> <a class='nav-link' href='index.html'>Home <span							class='sr-only'>(current)</span></a> </li>				<li class='nav-item'> <a class='nav-link' href='setting.html'>设置</a> </li>			<li class='nav-item'>					<a class='nav-link' href='now.html' id='time'></a>				</li>				</li>			</ul>		</div >";
document.getElementsByTagName("body")[0].appendChild(nav);
//This file is auto generate by QtBigScreen.
//Your changes will NOT be saved!!!
var li_0 = document.createElement('li');
li_0.className = 'nav-item'
var a_0 = document.createElement('a');
a_0.className = 'nav-link';
a_0.href = '/addon/About/about.html';
a_0.innerHTML = '关于';
li_0.appendChild(a_0);
document.getElementsByTagName('ul')[0].appendChild(li_0);
