all:
		cd src && $(MAKE)
		cp ./src/anzupop_mock_fs ./
		rm ./src/anzupop_mock_fs

clean:
		rm anzupop_mock_fs