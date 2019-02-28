package main

import (
	"flag"
	"fmt"
	"image/png"
	"io/ioutil"
	"os"
	"os/exec"
	"path/filepath"
	"strings"

	"github.com/nfnt/resize"
)

func main() {

	/*/我要实现的功能，、
	1 文件夹遍历一个数组
	2 文件解密
	3 文件分辨率修改
	4 文件压缩
	*/
	//findPath := "/Users/gaochao/Downloads/25945362208395264"
	// /Users/gaochao/Documents/go/src/ReSizePng/pngquant
	var findPath string
	var pngquantPath string
	flag.StringVar(&findPath, "path", "dirpath", "tell me the path(like -path=/Users/ReadingMate/Resources).")
	flag.StringVar(&pngquantPath, "pngquant", "pngquantno", "tell me the path(like -pngquant=/Users/ReadingMate/pngquant).")
	flag.Parse()

	if findPath == "dirpath" {
		logPrint("tell me the path(like -path=/Users/ReadingMate/Resources).")
		return
	}
	if pngquantPath == "pngquantno" {
		logPrint("tell me the path(like -pngquant=/Users/ReadingMate/pngquant).")
		return
	}
	//遍历
	logPrint("\ninit filepath start")
	rPath := getFilelist(findPath, ".png")
	//文件解密
	logPrint("\nfile decoding start")
	for i := 0; i < len(rPath); i++ {
		err := decoding(rPath[i])
		if err != nil {
			logPrint("decoding error:%d,%s", i, rPath[i])
			return
		}
		fmt.Print(".")
	}
	//分辨率处理
	logPrint("\npng scale start")
	for i := 0; i < len(rPath); i++ {
		err := pngscale(rPath[i])
		if err != nil {
			logPrint("pngscale error:%d,%s", i, rPath[i])
			return
		}
		fmt.Print(".")
	}
	//压缩
	logPrint("\npngquant start")
	for i := 0; i < len(rPath); i++ {
		err := pngquant(pngquantPath, rPath[i])
		if err != nil {
			logPrint("pngquant error:%d,%s", i, rPath[i])
			return
		}
		fmt.Print(".")
	}
	logPrint("\nrun over")
	return
}

//log输出
func logPrint(format string, a ...interface{}) {
	str := fmt.Sprintf(format, a...)
	fmt.Println(str)
}

//获取文件目录
func getFilelist(path string, exetend string) (rPath []string) {
	rPath = make([]string, 0, 100)
	err := filepath.Walk(path, func(path string, f os.FileInfo, err error) error {
		if f == nil {
			return err
		}
		if f.IsDir() {
			return nil
		}
		if strings.Contains(path, exetend) {
			rPath = append(rPath, path)
		}
		return nil
	})
	if err != nil {
		fmt.Printf("filepath.Walk() returned %v\n", err)
	}
	return rPath
}

//文件解密
func decoding(fileName string) error {
	// t := time.Now()
	KEYNUM := 3

	rw, err := os.OpenFile(fileName, os.O_RDWR, os.ModePerm)
	fileByte, err := ioutil.ReadAll(rw)
	if err != nil {
		return err
	}
	//判断是否加密
	head := string(fileByte[0:7])
	if head != "metrics" {
		return nil
	}
	_, file := filepath.Split(fileName)
	//生成KEY
	nkey := make([]byte, KEYNUM)
	for i := 0; i < KEYNUM*len(file); i++ {
		nkey[i%KEYNUM] += file[i%len(file)]
	}

	contentLen := len(fileByte)
	p := make([]byte, contentLen-8)

	for i := 0; i < contentLen-8; i++ {
		p[i] = fileByte[i+8] - nkey[(i+8)%KEYNUM]
	}

	// fmt.Println(p)
	ioutil.WriteFile(fileName, p, os.ModePerm)

	return nil
}

//png放缩
func pngscale(fileName string) error {
	//return nil
	// open "test.jpg"
	file, err := os.Open(fileName)
	if err != nil {
		return err
	}
	defer file.Close()

	img, err := png.Decode(file)
	if err != nil {
		return err
	}

	// resize to width 1000 using Lanczos resampling
	// and preserve aspect ratio
	m := resize.Resize(uint(img.Bounds().Size().X/2), 0, img, resize.Bilinear)

	out, err := os.Create(fileName)
	if err != nil {
		return err
	}
	defer out.Close()

	if err = png.Encode(out, m); err != nil {
		return nil
	}
	return nil
}

//png压缩
func pngquant(pngquant string, fileName string) error {
	cmd := exec.Command(pngquant, fileName, "--force", "--output", fileName)
	_, err := cmd.CombinedOutput()
	return err
}
