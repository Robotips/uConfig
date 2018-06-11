# Contributing to RTprog

:+1: We are happy to learn that you want to contribute !

Thank you for making the embedded world a more attractive domain. Each commits make it simpler.

This little guide goals to help you to contribute to this project.

Before contributing, never forget the following points:

* Keep the modularity of the project
* Keep the compatibility with a large set of microcontrollers
* Keep in mind RAM memory is expensive, use only that you really needs

## How Can I Contribute?

* Reporting bugs in [Issues](https://github.com/UniSwarm/uDevkit-IDE/issues)

* Pull requests in [Pull requests](https://github.com/UniSwarm/uDevkit-IDE/pulls)

## Styleguides

### Git commit messages

* Use past tense: `Updated funtion getSensor`

* Include the name of library or tool between square brackets.

  * `[kicad] msg` For a modification inside kicad library

### C++ code styleguide

* no tab, only spaces, 4 spaces
* headers files with .h extension, .cpp for code files
* class name start with a capital and are in camel case style
* private and protected members start with an underscore and a lowercase, camel case style
* getters have the same name than the member without the underscore
* setters start by set followed by the member name with a capital

example, projectloader.h:
```C++
class ProjectLoader
{
public:
	ProjectLoader();

	const QString &path() const;
	void setPath(const QString &path);

protected:
	QString _path;
};
```
