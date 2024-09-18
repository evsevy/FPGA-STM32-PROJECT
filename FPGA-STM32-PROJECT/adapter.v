module GyroProcessor (
    input wire clk,
    input wire reset,
    input wire [15:0] gyro_data,
    output reg [15:0] servo_roll,
    output reg [15:0] servo_pitch
);

always @(posedge clk or posedge reset) begin
    if (reset) begin
        servo_roll <= 16'b0;
        servo_pitch <= 16'b0;
    end else begin
        // Обработка данных гироскопа
        servo_roll <= gyro_data; // Здесь можно добавить фильтрацию
        servo_pitch <= gyro_data; // Здесь тоже
    end
end

endmodule

#VHDL

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity GyroProcessor is
    port (
        clk           : in std_logic;
        reset         : in std_logic;
        gyro_data     : in std_logic_vector(15 downto 0);
        servo_roll    : out std_logic_vector(15 downto 0);
        servo_pitch   : out std_logic_vector(15 downto 0)
    );
end entity GyroProcessor;

architecture Behavioral of GyroProcessor is
begin
    process(clk, reset)
    begin
        if reset = '1' then
            servo_roll <= (others => '0');
            servo_pitch <= (others => '0');
        elsif rising_edge(clk) then
            -- Обработка данных гироскопа
            servo_roll <= gyro_data; -- Здесь можно добавить фильтрацию
            servo_pitch <= gyro_data; -- Здесь тоже
        end if;
    end process;
end architecture Behavioral;