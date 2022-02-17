	component step5 is
		port (
			clk_clk               : in  std_logic                     := 'X'; -- clk
			ledr_export           : out std_logic_vector(9 downto 0);         -- export
			prime_number_readdata : out std_logic_vector(31 downto 0);        -- readdata
			reset_reset_n         : in  std_logic                     := 'X'  -- reset_n
		);
	end component step5;

	u0 : component step5
		port map (
			clk_clk               => CONNECTED_TO_clk_clk,               --          clk.clk
			ledr_export           => CONNECTED_TO_ledr_export,           --         ledr.export
			prime_number_readdata => CONNECTED_TO_prime_number_readdata, -- prime_number.readdata
			reset_reset_n         => CONNECTED_TO_reset_reset_n          --        reset.reset_n
		);

